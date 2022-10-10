// implement the matrix structure
#include <cmath>
#include <cassert>
#include "geometry.h"

Vec4f Vec3ToVec4(const Vec3f v, bool isPoint = true){
    if(isPoint){
        return Vec4f(v.x, v.y, v.z, 1.f);
    }
    else{
        return Vec4f(v.x, v.y, v.z, 0.f);
    }
}

Vec3f Vec4ToVec3(const Vec4f v){
    // vector
    if(abs(v.w) < 1e-6){
        return Vec3f(v.x, v.y, v.z);
    }
    // point
    else{
        return Vec3f(v.x/v.w, v.y/v.w, v.z/v.w);
    }
}

int Matrix::nrows(){ return this->rows;}
int Matrix::ncols(){ return this->cols;}

Matrix::Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC){
    this->rows = r;
    this->cols = c;
    data = std::vector<std::vector<float>>(r, std::vector<float>(c, 0.f));
}

std::vector<float>& Matrix::operator[](const int i){
    assert(i >= 0 && i < rows);
    return this->data[i];
}

Matrix Matrix::operator*(const Matrix &m){
    assert(this->cols == m.rows);
    Matrix result(this->rows, m.cols);
    for(int i = 0; i < this->rows; i++){
        for(int j = 0; j < m.cols; j++){
            for(int k = 0; k < this->cols; k++){
                result[i][j] += this->data[i][k] * m.data[k][j];
            }
        }
    }
    return result;
}

Vec4f Matrix::operator*(Vec4f vector){
    assert(this->rows == 4 && this->cols == 4);
    Vec4f result;
    for(int i = 0; i < this->rows; i++){
        for(int j = 0; j < this->cols; j++){
            result[i] += this->data[i][j] * vector[j];
        }
    }
    return result;
}

Matrix Matrix::transpose(){
    Matrix result(this->cols, this->rows);
    for(int i = 0; i < result.rows; i++){
        for(int j = 0; j < result.cols; j++){
            result[i][j] = this->data[j][i];
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix &m){
    assert(this->rows == m.rows && this->cols == m.cols);
    Matrix result(this->rows, this->cols);
    for(int i = 0; i < result.rows; i++){
        for(int j = 0; j < result.cols; j++){
            result[i][j] = this->data[i][j] + m.data[i][j];
        }
    }
    return result;
}

// too lazy to implement, just leave it when necessary
Matrix Matrix::inverse(){
    return Matrix(this->rows, this->cols);
}

Matrix Matrix::identity(int dimensions = DEFAULT_ALLOC){
    Matrix result(dimensions, dimensions);
    for(int i = 0; i < dimensions; i++){
        result[i][i] = 1.0f;
    }
    return result;
}

Matrix Matrix::zeros(int dimensions = DEFAULT_ALLOC){
    Matrix result(dimensions, dimensions);
    return result;
}

std::ostream& operator<<(std::ostream &s, Matrix &m){
    for(int i = 0; i < m.nrows(); i++){
        for(int j = 0; j < m.ncols(); j++){
            s << m[i][j];
            if(j != m.ncols()-1) s << '\t';
        }
        s << '\n';
    }
    return s;
}

// provide functions to produce transformation matrix(4 * 4)
Matrix Matrix::modelTrans(){
    // currently have no idea, just return identity
    return Matrix::identity();
}

// here, width and height refer to the physical screen / output image pixel number
Matrix Matrix::viewportTrans(const int width, const int height){
    Matrix view = Matrix::identity();
    view[0][0] = (float)width / 2;
    view[1][1] = (float)height / 2;
    view[0][3] = (float)(width-1) / 2;
    view[1][3] = (float)(height-1)/2;
    return view;
}
