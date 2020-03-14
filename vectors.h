//
// Created by Alexis on 29/01/2020.
//

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

/**
 * We define our different vectors here ; 3D-coordinates in space, 2D-coordinates on the image, etc...
 */

template <size_t DIM, typename T> struct vec {
    vec() { for (size_t i=DIM; i--; data_[i] = T()); }
    T& operator[](const size_t i)       { assert(i<DIM); return data_[i]; }
    const T& operator[](const size_t i) const { assert(i<DIM); return data_[i]; }
private:
    T data_[DIM];
};

typedef vec<3, float> Vec3f;
typedef vec<2, float> Vec2f;
typedef vec<3, int> Vec3i;
typedef vec<2, int> Vec2i;

template <typename T> struct vec<3,T> {
    vec() : x(T()), y(T()), z(T()) {}
    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    T& operator[](const size_t i)       { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    const T& operator[](const size_t i) const { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    float norm() { return std::sqrt(x*x+y*y+z*z); }
    vec<3,T> & normalize(T l=1) { *this = (*this)*(l/norm()); return *this; }
    T x,y,z;
};

template <typename T> struct vec<2,T> {
    vec() : x(T()), y(T()) {}
    vec(T X, T Y) : x(X), y(Y) {}
    T& operator[](const size_t i)       { assert(i<2); return i<=0 ? x : y; }
    const T& operator[](const size_t i) const { assert(i<2); return i<=0 ? x : y; }
    float norm() { return std::sqrt(x*x+y*y); }
    vec<2,T> & normalize(T l=1) { *this = (*this)*(l/norm()); return *this; }
    T x,y;
};


template<size_t DIM,typename T> T operator*(const vec<DIM,T>& lhs, const vec<DIM,T>& rhs) {
    T ret = T();
    for (size_t i=DIM; i--; ret+=lhs[i]*rhs[i]);
    return ret;
}

template<size_t DIM,typename T>vec<DIM,T> operator+(vec<DIM,T> lhs, const vec<DIM,T>& rhs) {
    for (size_t i=DIM; i--; lhs[i]+=rhs[i]);
    return lhs;
}

template<size_t DIM,typename T>vec<DIM,T> operator-(vec<DIM,T> lhs, const vec<DIM,T>& rhs) {
    for (size_t i=DIM; i--; lhs[i]-=rhs[i]);
    return lhs;
}

template<size_t DIM,typename T,typename U> vec<DIM,T> operator*(const vec<DIM,T> &lhs, const U& rhs) {
    vec<DIM,T> ret;
    for (size_t i=DIM; i--; ret[i]=lhs[i]*rhs);
    return ret;
}

template<size_t DIM,typename T> vec<DIM,T> operator-(const vec<DIM,T> &lhs) {
    return lhs*T(-1);
}

template <typename T> vec<3,T> cross(vec<3,T> v1, vec<3,T> v2) {
    return vec<3,T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& out, const vec<DIM,T>& v) {
    for(unsigned int i=0; i<DIM; i++) out << v[i] << " " ;
    return out ;
}

// Triangles definition.

template <size_t DIM, typename K> struct polygon {
  polygon() {for (size_t i=DIM; i--; data_[i] = K());}
  K& operator[](const size_t i) {assert(i<DIM); return data_[i];}
  const K& operator[](const size_t i) const {assert(i<DIM); return data_[i];}
private:
  K data_[DIM];
};

typedef polygon<3,Vec2i> Triangle2i;
typedef polygon<3,Vec2f> Triangle2f;
typedef polygon<3,Vec3f> Triangle3f;

template <typename K> struct polygon<3,K> {
  polygon() : v0(K()), v1(K()), v2(K()) {}
  polygon(K V0, K V1, K V2) : v0(V0), v1(V1), v2(V2) {}
  const K& operator[](const size_t i) const {assert(i<3); return i<=0 ? v0 : (i==1 ? v1 : v2); }
  K& operator[](const size_t i) {assert(i<3); return i<=0 ? v0 : (i==1 ? v1 : v2); }
  Vec3f barycentric(K p) {
    Vec3f c = cross(Vec3f(v2.x-v0.x,v1.x-v0.x,v0.x-p.x),Vec3f(v2.y-v0.y,v1.y-v0.y,v0.y-p.y));
    if(std::abs(c.z)<1) return Vec3f(-1,1,1); //Return negative coordinates if triangle is degenerate
    return Vec3f(1.f-(c.x+c.y)/c.z,c.y/c.z,c.x/c.z);
  }
  K v0,v1,v2;
};

struct TexturedTriangle{
  TexturedTriangle(Triangle3f wV, Triangle2f tV) : worldVertices(wV), textureVertices(tV) {} 
  Triangle3f worldVertices;
  Triangle2f textureVertices;
};


// Matrices definition.

const int DEF_DIM=4;

class Matrix{
  
public:
  Matrix(int w=DEF_DIM, int h=DEF_DIM);
  static Matrix identity(int DIM=DEF_DIM);
  Matrix transpose();
  Matrix inverse();
  int get_width() const;
  int get_height() const;
  Matrix operator*(const Matrix& m);
  std::vector<float>& operator[](const int i);
  friend std::ostream& operator<<(std::ostream& s, const Matrix &m);
private:
  int width_,height_;
  std::vector<std::vector<float>> m_;
};

#endif
