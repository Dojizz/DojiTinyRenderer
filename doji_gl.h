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

#endif
