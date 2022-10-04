#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <vector>
#include <cmath>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model *model = NULL;
const int width = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
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

// draw the triangle using barycentric coordinate
void triangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage &image, TGAColor color){
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
            // if (x, y) in triangle, v0->A, v1->B, v2->C
            Vec3f vector1(v0.x - x, v1.x - v0.x, v2.x - v0.x);
            Vec3f vector2(v0.y - y, v1.y - v0.y, v2.y - v0.y);

            // cross product to get (1, u, v)
            Vec3f vector3 = vector1 ^ vector2;
            // vector3.x = vector1.y * vector2.z - vector1.z * vector2.y;
            // vector3.y = vector1.z * vector2.x - vector1.x * vector2.z;
            // vector3.z = vector1.x * vector2.y - vector1.y * vector2.x;
            vector3 = vector3 * (1 / vector3.x);

            if(vector3.y >=0 && vector3.z >= 0 && vector3.y + vector3.z <= 1)
                image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {

    if(argc >= 2){
        model = new Model(argv[1]);
    }
    else
        model = new Model("obj/african_head.obj");

    TGAImage image(width, height, TGAImage::RGB);
    Vec3f light_dir(0, 0, -1);
    
    // Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    // Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    // Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    // triangle(t0[0], t0[1], t0[2], image, white);
    // triangle(t1[0], t1[1], t1[2], image, green);
    // triangle(t2[0], t2[1], t2[2], image, red);
    for(int i=0; i < model->nfaces(); i++){
        // fetch point data
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for(int j=0; j<3; j++){
            world_coords[j] = model->vert(face[j]);
            screen_coords[j] = Vec2i((world_coords[j].x + 1.)*width/2., (world_coords[j].y+1.)*height/2.);
        }

        // calculate light intensity
        Vec3f vector1 = world_coords[1] - world_coords[0];
        Vec3f vector2 = world_coords[2] - world_coords[0];
        Vec3f normal = vector2 ^ vector1;
        normal.normalize();
        float intensity = normal * light_dir;
        if(intensity > 0){
            TGAColor c(255 * intensity, 255 * intensity, 255 * intensity, 255);
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, c);
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}