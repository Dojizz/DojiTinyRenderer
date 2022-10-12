#include "doji_gl.h"

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

Vec3f barycentric(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Vec2i &p){
    // no need to get confused by math, just use!
    Vec3f vector1(v0.x - p.x, v1.x - v0.x, v2.x - v0.x);
    Vec3f vector2(v0.y - p.y, v1.y - v0.y, v2.y - v0.y);

    Vec3f vector3 = vector1 ^ vector2;
    vector3 = vector3 * (1 / vector3.x);
    vector3.x = 1 - vector3.y - vector3.z;
    return vector3;
}

// draw the triangle using barycentric coordinate
void triangle(Vec3f v0, Vec3f v1, Vec3f v2,
              Vec3f t0, Vec3f t1, Vec3f t2,
              TGAImage &image, float intensity, std::vector<float> &zbuffer, TGAImage &texture,
              int width, int height){
    // get boouding box
    Vec2f maxV, minV;
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
    for(int x = (int)minV.x; x <= (int)maxV.x; x++){
        for(int y = (int)minV.y; y <= (int)maxV.y; y++){
            Vec3f barycentric_coord = barycentric(v0, v1, v2, Vec2i(x, y));

            // pixel in the triangle
            if(barycentric_coord.y >=0 && barycentric_coord.z >= 0 && barycentric_coord.x >= 0){
                // get the depth 
                float z = barycentric_coord * Vec3f(v0.z, v1.z, v2.z);
                // float z = barycentric_coord.x * v0.z + barycentric_coord.y * v1.z + barycentric_coord.z * v2.z;
                // get the texture coordinate
                float u = barycentric_coord * Vec3f(t0.u, t1.u, t2.u);
                // float u = barycentric_coord.x * t0.u + barycentric_coord.y * t1.u + barycentric_coord.z * t2.u;
                u *= texture.get_width();
                float v = barycentric_coord * Vec3f(t0.v, t1.v, t2.v);
                // float v = barycentric_coord.x * t0.v + barycentric_coord.y * t1.v + barycentric_coord.z * t2.v;
                v *= texture.get_height();
                if(z > zbuffer[x * width + y]){
                    TGAColor color = texture.get((int)u, (int)v);
                    color = color * intensity;
                    image.set(x, y, color);
                    // generate depth map
                    // int z_color = (z + 1) * 255 / 2;
                    // image.set(x, y, TGAColor(z_color, z_color, z_color, 255));
                    zbuffer[x * width + y] = z;
                }
            }
        }
    }
}