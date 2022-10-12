#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <vector>
#include <iostream>
#include <cassert>
#define PI 3.14159265

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class t> struct Vec2 {
	union {
		struct {t u, v;};
		struct {t x, y;};
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u),v(_v) {}
	inline Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(u+V.u, v+V.v); }
	inline Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(u-V.u, v-V.v); }
	inline Vec2<t> operator *(float f)          const { return Vec2<t>(u*f, v*f); }
	t& operator[](const int i){
		assert(i >= 0 && i <=1);
		return raw[i];
	}
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
	union {
		struct {t x, y, z;};
		struct {t u, v, w;};
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x),y(_y),z(_z) {}
	// cross product
	inline Vec3<t> operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline Vec3<t> operator +(const Vec3<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
	inline Vec3<t> operator -(const Vec3<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
	inline Vec3<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
	inline t       operator *(const Vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
	t& operator[](const int i) {
		assert(i >= 0 && i <= 2);
		return raw[i];
	}
	float norm () const { return std::sqrt(x*x+y*y+z*z); }
	Vec3<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

template <class t> struct Vec4 {
	union
	{
		struct{t x, y, z, w;};
		t raw[4];
	};
	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}
	inline Vec4<t> operator +(const Vec4<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z, w+v.w); }
	inline Vec4<t> operator -(const Vec4<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z, w-v.w); }
	inline Vec4<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f, w*f); }
	t& operator[](const int i) {
		assert(i >= 0 && i <= 3);
		return raw[i];
	}
	float norm () const { return std::sqrt(x*x+y*y+z*z);}
	// for w = 0, direction vector, before do this, you better have some b nubmer
	Vec4<t> & vectorNormalize(t l=1) {*this = *this *(l/norm()); return *this;}
	// for w != 0, point vector
	Vec4<t> & pointNormalize() {*this = *this *(1/w); return *this;}
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec4<t>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;
typedef Vec4<float> Vec4f;

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec4<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
	return s;
}

// vec3f to vec4f
Vec4f Vec3ToVec4(const Vec3f v, bool isPoint = true);
// inverse
Vec3f Vec4ToVec3(const Vec4f v);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int DEFAULT_ALLOC = 4;

class Matrix {
	private:
		std::vector<std::vector<float>> data;
		int rows, cols;
	public:
		inline int nrows();
		inline int ncols();
		// construct
		Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
		// transpose
		Matrix transpose();
		// [][] operator
		std::vector<float>& operator[](const int i);
		// * operator with matrix
		Matrix operator*(const Matrix &m);
		// * operator with vector, only support 4x4 * 4*1 
		Vec4f operator*(Vec4f vector);
		// + operator
		Matrix operator+(const Matrix &m);
		// inverse
		Matrix inverse();
		// generate some specific matrix
		static Matrix identity(int dimensions = DEFAULT_ALLOC);
		static Matrix zeros(int dimention = DEFAULT_ALLOC);
		static Matrix modelTrans();
		static Matrix viewportTrans(const int width, const int height);
		// output
		friend std::ostream& operator<<(std::ostream &s, Matrix &m);

};

#endif //__GEOMETRY_H__