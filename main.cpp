#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "camera.h"
#include "doji_gl.h"
#include <vector>
#include <cmath>
#include <iostream>

// define default model, texture path
const char *modelPath = "obj/african_head.obj";
const char *texturePath = "texture/african_head_diffuse.tga";
// define image
const int width = 800;
const int height = 800;
const int smallest_depth = -1000;
// define camera
Camera camera(Vec3f(1.5f, 1.5f, 2.5f), Vec3f(-1.f, -1.f, -2.f));
Vec3f light_dir(0.f, 0.f, -1.f);

// define model matrix
Matrix modelTrans = Matrix::modelTrans();
Matrix cameraTrans = camera.getCameraTrans();
Matrix projTrans = camera.getProjTrans();
Matrix viewportTrans = Matrix::viewportTrans(width, height);
Matrix transformation = viewportTrans * projTrans * cameraTrans * modelTrans;

int main(int argc, char** argv) {

    // load model
    Model *model = nullptr;
    if(argc >= 2){
        model = new Model(argv[1]);
    }
    else
        model = new Model(modelPath); 

    // load texture, set parameter
    TGAImage texture;
    texture.read_tga_file(texturePath);
    texture.flip_vertically();

    light_dir.normalize();

    // define z-buffer, image object
    std::vector<float> zbuffer(width * height, smallest_depth);
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage zbuffer1(width, height, TGAImage::GRAYSCALE);

    // draw per faces
    for(int i=0; i < model->nfaces(); i++){
        // fetch point data
        std::vector<int> face = model->face(i);
        // coords in screen space
        Vec3f screen_coords[3];
        // coords in model space
        Vec3f model_coords[3];
        Vec3f texture_coords[3];
        for(int j=0; j<3; j++){
            model_coords[j] = model->vert(face[2*j]);
            // transfer local model coords to screen space coords 
            screen_coords[j] = Vec4ToVec3(transformation * Vec3ToVec4(model_coords[j]));
            // must get texture coords of each vertex at here
            texture_coords[j] = model->texture(face[2*j+1]);
        }

        // calculate light intensity, normal points to the inner side
        Vec3f vector1 = model_coords[1] - model_coords[0];
        Vec3f vector2 = model_coords[2] - model_coords[0];
        Vec3f normal = vector2 ^ vector1;
        normal.normalize();
        float intensity = normal * light_dir;
        
        //TGAColor c(255 * intensity, 255 * intensity, 255 * intensity, 255);
        triangle(screen_coords[0], screen_coords[1], screen_coords[2],
                    texture_coords[0], texture_coords[1], texture_coords[2],
                    image, intensity, zbuffer, texture, width, height);
        
    }

    // flip along the horizontal axis
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("image/output.tga");

    delete model;
    return 0;
}