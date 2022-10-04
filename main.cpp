#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <vector>
#include <cmath>
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const TGAColor blue  = TGAColor(0,   0,   255, 255);
Model *model = NULL;
const int width = 800;
const int height = 800;

void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color) { 
    int x0 = v0.x, y0 = v0.y, x1 = v1.x, y1 = v1.y;
    bool steep = false;
    if(std::abs(x0-x1) < std::abs(y0-y1)){
        // always iterate through x axis
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    // iterate from small to large
    if(x0 > x1){
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = 2 * std::abs(dy);
    int error = 0;
    int y = y0;

    // error indicates the distance between true line and
    // pixel center to draw
    for(int x = x0; x <= x1; x++){
        if(steep){
            image.set(y, x, color);
        }
        else{
            image.set(x, y, color);
        }
        // move to the next pixel
        error += derror;
        if(error > dx){
            y += (y1>y0?1:-1);
            error -= dx*2;
        }
    }
}

Vec3f barycentric(const Vec3i &v0, const Vec3i &v1, const Vec3i &v2, const Vec2i &p){
    // no need to get confused by math, just use!
    Vec3f vector1(v0.x - p.x, v1.x - v0.x, v2.x - v0.x);
    Vec3f vector2(v0.y - p.y, v1.y - v0.y, v2.y - v0.y);

    Vec3f vector3 = vector1 ^ vector2;
    vector3 = vector3 * (1 / vector3.x);
    vector3.x = 1 - vector3.y - vector3.z;
    return vector3;
}

// draw the triangle using barycentric coordinate
void triangle(Vec3i v0, Vec3i v1, Vec3i v2,
              Vec3f t0, Vec3f t1, Vec3f t2,
              TGAImage &image, float intensity, std::vector<float> &zbuffer, TGAImage &texture){
    // get boouding box
    Vec2i maxV, minV;
    // max (x, y)
    maxV.x = v0.x > v1.x ? v0.x : v1.x;
    maxV.x = maxV.x > v2.x ? maxV.x : v2.x;
    maxV.y = v0.y > v1.y ? v0.y : v1.y;
    maxV.y = maxV.y > v2.y ? maxV.y : v2.y;
    // min {x, y}
    minV.x = v0.x < v1.x ? v0.x : v1.x;
    minV.x = minV.x < v2.x ? minV.x : v2.x;
    minV.y = v0.y < v1.y ? v0.y : v1.y;
    minV.y = minV.y < v2.y ? minV.y : v2.y;

    // traverse in bouding box
    for(int x = minV.x; x <= maxV.x; x++){
        for(int y = minV.y; y <= maxV.y; y++){
            Vec3f barycentric_coord = barycentric(v0, v1, v2, Vec2i(x, y));

            // pixel in the triangle
            if(barycentric_coord.y >=0 && barycentric_coord.z >= 0 && barycentric_coord.x >= 0){
                // get the depth 
                float z = barycentric_coord.x * v0.z + barycentric_coord.y * v1.z + barycentric_coord.z * v2.z;
                // get the texture coordinate
                float u = barycentric_coord.x * t0.u + barycentric_coord.y * t1.u + barycentric_coord.z * t2.u;
                u *= texture.get_width();
                float v = barycentric_coord.x * t0.v + barycentric_coord.y * t1.v + barycentric_coord.z * t2.v;
                v *= texture.get_height();
                if(z > zbuffer[x * width + y]){
                    TGAColor color = texture.get((int)u, (int)v);
                    color = color * intensity;
                    image.set(x, y, color);
                    zbuffer[x * width + y] = z;
                }
            }
        }
    }
}

int main(int argc, char** argv) {

    // load model
    if(argc >= 2){
        model = new Model(argv[1]);
    }
    else
        model = new Model("obj/african_head.obj"); 

    // load texture, set parameter
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage texture;
    texture.read_tga_file("texture/african_head_diffuse.tga");
    texture.flip_vertically();
    std::vector<float> zbuffer(width * height, -1000);
    Vec3f light_dir(0, 0, -1);

    // draw per faces
    for(int i=0; i < model->nfaces(); i++){
        // fetch point data
        std::vector<int> face = model->face(i);
        Vec3i screen_coords[3];
        Vec3f world_coords[3];
        Vec3f texture_coords[3];
        for(int j=0; j<3; j++){
            world_coords[j] = model->vert(face[2*j]);
            screen_coords[j] = Vec3i((world_coords[j].x + 1.)*width/2., (world_coords[j].y+1.)*height/2., (int)world_coords[j].z);
            // must get texture coords of each vertex at here
            texture_coords[j] = model->texture(face[2*j+1]);
        }

        // calculate light intensity
        Vec3f vector1 = world_coords[1] - world_coords[0];
        Vec3f vector2 = world_coords[2] - world_coords[0];
        Vec3f normal = vector2 ^ vector1;
        normal.normalize();
        float intensity = normal * light_dir;
        if(intensity > 0){
            //TGAColor c(255 * intensity, 255 * intensity, 255 * intensity, 255);
            triangle(screen_coords[0], screen_coords[1], screen_coords[2],
                     texture_coords[0], texture_coords[1], texture_coords[2],
                     image, intensity, zbuffer, texture);
        }
    }

    // flip along the horizontal axis
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}