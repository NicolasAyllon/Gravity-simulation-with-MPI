#ifndef _VECTOR_H
#define _VECTOR_H

#include <cmath>

template <typename T>
struct Vec2 {
  T x;
  T y;

  Vec2() = default;
  Vec2(T x, T y);
  Vec2(const Vec2& v) = default;
  Vec2& operator=(const Vec2& rhs);
};

////////////////////////////////////////////////////////////////////////////////
// Constructors & Assignment
////////////////////////////////////////////////////////////////////////////////
template <typename T>
Vec2<T>::Vec2(T x_in, T y_in) : x(x_in), y(y_in) {}

template <typename T>
Vec2<T>& Vec2<T>::operator=(const Vec2& rhs) {
  this.x = rhs.x;
  this.y = rhs.y;
  return this;
}

////////////////////////////////////////////////////////////////////////////////
// Operations
////////////////////////////////////////////////////////////////////////////////

// Vector addition
template <typename T>
Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2) {
  return Vec2{
    v1.x + v2.x,
    v1.y + v2.y
  };
}

// Vector subtraction
template <typename T>
Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2) {
  return Vec2{
    v1.x - v2.x,
    v1.y - v2.y
  };
}

// Same Type
// Right multiplication of vector v by scalar k: vk
template <typename T>
Vec2<T> operator*(const Vec2<T>& v, T k) {
  return Vec2{
    v.x * k,
    v.y * k
  };
}

// Same Type
// Left multiplication of vector v by scalar k: kv
template <typename T>
Vec2<T> operator*(T k, const Vec2<T>& v) {
  return Vec2{
    k * v.x,
    k * v.y
  };
}

// Vector division by scalar: v/k
template <typename T>
Vec2<T> operator/(const Vec2<T>& v, T k) {
  return Vec2{
    v.x/k,
    v.y/k
  };
}

////////////////////////////////////////////////////////////////////////////////
// Operation & Assignment
////////////////////////////////////////////////////////////////////////////////

// Addition assign
template <typename T>
Vec2<T>& operator+=(Vec2<T>& v, const Vec2<T>& rhs) {
  v.x += rhs.x;
  v.y += rhs.y;
  return v;
}

// Subtraction assign
template <typename T>
Vec2<T>& operator-=(Vec2<T>&v, const Vec2<T>& rhs) {
  v.x -= rhs.x;
  v.y -= rhs.y;
  return v;
}

// Multiplication assign (right multiply)
template <typename T>
Vec2<T>& operator*=(Vec2<T>&v, T k) {
  v.x *= k;
  v.y *= k;
  return v;
}

// Division assign
template <typename T>
Vec2<T>& operator/=(Vec2<T>&v, T k) {
  v.x /= k;
  v.y /= k;
  return v;
}

// [?] Can use different types: Vec2<double> v{4.5, 7.6}; int k = 2; v/k = ?

////////////////////////////////////////////////////////////////////////////////
// Compare
////////////////////////////////////////////////////////////////////////////////
template <typename T>
bool operator==(const Vec2<T>& v1, const Vec2<T>& v2) {
  return v1.x == v2.x && 
         v1.y == v2.y;
}

template <typename T>
bool operator!=(const Vec2<T>& v1, const Vec2<T>& v2) {
  return !(v1 == v2);
}

////////////////////////////////////////////////////////////////////////////////
// Length, Dot Product & Distance
////////////////////////////////////////////////////////////////////////////////

// Returns the dot product of vectors a and b
template <typename T>
T dot(const Vec2<T>& a, const Vec2<T>& b) {
  return (a.x*b.x + a.y*b.y);
}

// Returns the squared length of vector v
// Equivalent to v.x*v.x + v.y*v.y
template <typename T>
T len2(const Vec2<T>& v) {
  return dot(v, v);
}

// Returns the length of vector v
// Equivalent to √(v.x*v.x + v.y*v.y)
template <typename T>
T len(const Vec2<T>& v) {
  return std::sqrt(len2(v));
}

// Returns the squared distance between vectors a and b
// Equivalent to (b.x - a.x)^2 + (b.y - a.y)^2
template <typename T>
T dist2(const Vec2<T>& a, const Vec2<T>& b) {
  return len2(b - a);
}

// Returns the distance between vectors a and b
// Equivalent to √[(b.x - a.x)^2 + (b.y - a.y)^2]
template <typename T>
T dist(const Vec2<T>& a, const Vec2<T>& b) {
  return len(b - a);
}

#endif //_VECTOR_H