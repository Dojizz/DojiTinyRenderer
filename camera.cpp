#include <cmath>
#include "geometry.h"
#include "camera.h"

Camera::Camera(Vec3f c , Vec3f l , Vec3f u ,
               float n , float f , float t ,
               float r , projectionClass p){
    this->centerPosition = c;
    this->lookAt = l;
    this->up = u;
    this->near = n;
    this->far = f;
    this->theta = t;
    this->ratio = r;
    this->proj = p;
    // h/2 = -near * tan(theta/2)
    this->height = -this->near * tan(90.f * this->theta / PI);
    this->height *= 2;
    this->width = this->height * this->ratio;
}

Matrix Camera::getCameraTrans(){
    Vec3f w = this->lookAt * (-1.f);
    w.normalize();
    Vec3f u = this->up ^ w;
    u.normalize();
    Vec3f v = w ^ u;
    v.normalize();
    Matrix moveToCenter = Matrix::identity();
    for(int i = 0; i < 3; i++) moveToCenter[i][3] = -this->centerPosition[i];
    Matrix moveDirec = Matrix::zeros();
    moveDirec[3][3] = 1.f;
    for(int i = 0; i < 3; i++){
        moveDirec[0][i] = u[i];
        moveDirec[1][i] = v[i];
        moveDirec[2][i] = w[i];
    }
    return moveDirec * moveToCenter;
}

Matrix Camera::getProjTrans(){
    // this operation can lead to vec[3] != 1!
    Matrix persTrans = Matrix::zeros();
    // if perspective, need to transfer to cube first
    if(this->proj == PERSPECTIVE){
        persTrans[0][0] = this->near;
        persTrans[1][1] = this->near;
        persTrans[2][2] = this->near + this->far;
        persTrans[2][3] = -this->near * this->far;
        persTrans[3][2] = 1;
    }
    else{
        persTrans = Matrix::identity();
    }
    // conduct orthogonal transformation
    Matrix orthoTrans = Matrix::zeros();
    orthoTrans[0][0] = 2.f/this->width;
    orthoTrans[1][1] = 2.f/this->height;
    orthoTrans[2][2] = 2.f/(this->near - this->far);
    orthoTrans[2][3] = -(this->near + this->far)/(this->near - this->far);
    orthoTrans[3][3] = 1;
    return orthoTrans * persTrans;
}