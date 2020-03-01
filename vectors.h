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

#endif
