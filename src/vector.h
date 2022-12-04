#ifndef _VECTOR_H
#define _VECTOR_H

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

// Right multiplication of vector v by scalar k: vk
template <typename T>
Vec2<T> operator*(const Vec2<T>& v, T k) {
  return Vec2{
    v.x * k,
    v.y * k
  };
}

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

#endif