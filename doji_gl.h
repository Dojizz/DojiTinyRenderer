// matrix initialization function

// triangle resterization

#ifndef __DOJI_GL_H__
#define __DOJI_GL_H__

#include "geometry.h"
#include "tgaimage.h"

void line(Vec2i v0, Vec2i v1, TGAImage &image, TGAColor color);

Vec3f barycentric(const Vec3f &v0, const Vec3f &v1, const Vec3f &v2, const Vec2i &p);

void triangle(Vec3f v0, Vec3f v1, Vec3f v2,
              Vec3f t0, Vec3f t1, Vec3f t2,
              TGAImage &image, float intensity, std::vector<float> &zbuffer, TGAImage &texture,
              int width, int height);

// matrix defined in main, will be used in shader
extern Matrix modelTrans;
extern Matrix cameraTrans;
extern Matrix projTrans;
extern Matrix viewportTrans;
extern Matrix transformation;

// abstract shader class
class DojiShader{
    virtual ~DojiShader();
    virtual Vec3f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec3f bary, TGAColor &color) = 0;
};

void triangle1(Vec4f *pts, DojiShader &shader, TGAImage &image, TGAImage &zbuffer);

#endif
