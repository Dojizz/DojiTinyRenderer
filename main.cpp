#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <vector>
#include <cmath>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
// Model *model = NULL;
const int width = 200;
const int height = 200;

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

void triangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage &image, TGAColor color){
    if(v0.x == v1.x && v1.x == v2.x) return;
    // draw the triangle by column, first sort the point by x values
    if(v0.x > v1.x) std::swap(v0, v1);
    if(v0.x > v2.x) std::swap(v0, v2);
    if(v1.x > v2.x) std::swap(v1, v2);

    int total_width = v2.x - v0.x;
    for(int x = v0.x; x <= v2.x; x++){
        // check if phase one or phase two
        bool if_second_phase = x >= v1.x;
        int part_width = if_second_phase ? v2.x - v1.x : v1.x - v0.x;
        Vec2i firstV = if_second_phase ? v1 : v0;
        Vec2i secondV = if_second_phase ? v2 : v1;

        float t02 = (x - v0.x)/(float)(total_width);
        int y02 = v0.y + (v2.y-v0.y) * t02;
        float t = (x - firstV.x)/(float)(part_width);
        int y_ = firstV.y + (secondV.y - firstV.y) * t;

        if(y_ > y02) std::swap(y_, y02);
        for(int y = y_; y <= y02; y++)
            image.set(x, y, color);

    }

    // // when v0.x == v1.x, skip the first part
    // if(v0.x == v1.x){
    //     int y0 = v0.y < v1.y ? v0.y : v1.y;
    //     int y1 = v0.y > v1.y ? v0.y : v1.y;
    //     for(int y = y0; y <= y1; y++){
    //         image.set(v0.x, y, color);
    //     }
    // }

    // // draw the first part
    // else{
    //     for(int x = v0.x; x <= v1.x; x++){
    //         float t01 = (x - v0.x)/(float)(v1.x-v0.x);
    //         int y01 = v0.y + (v1.y-v0.y)*t01;
    //         float t02 = (x - v0.x)/(float)(v2.x-v0.x);
    //         int y02 = v0.y + (v2.y-v0.y)*t02;
            
    //         // draw from small to large
    //         if(y01 > y02)
    //             std::swap(y01, y02);
    //         for(int y=y01; y<=y02; y++)
    //             image.set(x, y, color);
    //     }
    // }
    
    // // if v1.x == v2.x, skip the second part
    // if(v1.x == v2.x){
    //     int y0 = v1.y < v2.y ? v1.y : v2.y;
    //     int y1 = v1.y > v2.y ? v1.y : v2.y;
    //     for(int y = y0; y <= y1; y++){
    //         image.set(v1.x, y, color);
    //     }
    // }
    // else{
    //     for(int x = v1.x+1; x <= v2.x; x++){
    //         float t02 = (x - v0.x)/(float)(v2.x-v0.x);
    //         int y02 = v0.y + (v2.y - v0.y)*t02;
    //         float t12 = (x - v1.x)/(float)(v2.x-v1.x);
    //         int y12 = v1.y + (v2.y - v1.y)*t12;

    //         if(y02 > y12)
    //             std::swap(y02, y12);
    //         for(int y=y02; y<=y12; y++){
    //             image.set(x, y, color);
    //         }
    //     }
    // }
}

int main(int argc, char** argv) {

    TGAImage image(width, height, TGAImage::RGB);
    
    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
    triangle(t0[0], t0[1], t0[2], image, white);
    triangle(t1[0], t1[1], t1[2], image, green);
    triangle(t2[0], t2[1], t2[2], image, red);
    
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}