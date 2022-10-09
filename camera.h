#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "geometry.h"

enum projectionClass {ORTHOGONAL, PERSPECTIVE};
const Vec3f DEFAULT_CENTER(0.f, 0.f, 0.f);
const Vec3f DEFAULT_LOOKAT(0.f, 0.f, -1.f);
const Vec3f DEFAULT_UP(0.f, 1.f, 0.f);
const float DEFAULT_NEAR = -1, DEFAULT_FAR = -100;
const float DEFAULT_THETA = 60, DEFAULT_RATIO = 1.0;
const projectionClass DEFAULT_PROJ = PERSPECTIVE;

class Camera{
    private:
        // center position in world coordinate
        Vec3f centerPosition;
        Vec3f lookAt;
        Vec3f up;
        float near, far;
        // unit is degree
        float theta;
        // width / height
        float ratio;
        // projection class
        projectionClass proj;
        // specific weight & height
        float width, height;

    public:
        Camera(Vec3f c = DEFAULT_CENTER, Vec3f l = DEFAULT_LOOKAT, Vec3f u = DEFAULT_UP,
               float n = DEFAULT_NEAR, float f = DEFAULT_FAR, float t = DEFAULT_THETA,
               float r = DEFAULT_RATIO, projectionClass p = DEFAULT_PROJ);

        Matrix getCameraTrans();
        Matrix getProjTrans();
};

#endif