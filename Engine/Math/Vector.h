#pragma once

#include "MathTypes.h"
#include <immintrin.h> // SIMD intrinsics
#include <iostream>

namespace Engine {
namespace Math {

// Forward declarations
struct Vec2;
struct Vec3;
struct Vec4;

//============================================================================
// Vec2 - 2D Vector
//============================================================================
struct Vec2 {
  float x, y;

  // Constructors
  Vec2() : x(0.0f), y(0.0f) {}
  Vec2(float scalar) : x(scalar), y(scalar) {}
  Vec2(float x, float y) : x(x), y(y) {}

  // Static factory methods
  static Vec2 Zero() { return Vec2(0.0f, 0.0f); }
  static Vec2 One() { return Vec2(1.0f, 1.0f); }
  static Vec2 UnitX() { return Vec2(1.0f, 0.0f); }
  static Vec2 UnitY() { return Vec2(0.0f, 1.0f); }

  // Operators
  Vec2 operator+(const Vec2 &other) const {
    return Vec2(x + other.x, y + other.y);
  }
  Vec2 operator-(const Vec2 &other) const {
    return Vec2(x - other.x, y - other.y);
  }
  Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
  Vec2 operator/(float scalar) const { return Vec2(x / scalar, y / scalar); }
  Vec2 operator-() const { return Vec2(-x, -y); }

  Vec2 &operator+=(const Vec2 &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vec2 &operator-=(const Vec2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Vec2 &operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  Vec2 &operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }

  float &operator[](int index) { return (&x)[index]; }
  const float &operator[](int index) const { return (&x)[index]; }

  // Vector operations
  float Dot(const Vec2 &other) const { return x * other.x + y * other.y; }
  float Length() const { return Sqrt(x * x + y * y); }
  float LengthSquared() const { return x * x + y * y; }

  Vec2 Normalized() const {
    float len = Length();
    return IsNearZero(len) ? Vec2::Zero() : *this / len;
  }

  void Normalize() {
    float len = Length();
    if (!IsNearZero(len)) {
      x /= len;
      y /= len;
    }
  }

  Vec2 Perpendicular() const { return Vec2(-y, x); }
  float Angle() const { return Atan2(y, x); }
};

//============================================================================
// Vec3 - 3D Vector with SIMD optimization
//============================================================================
struct alignas(16) Vec3 {
  union {
    struct {
      float x, y, z;
    };
    float data[3];
  };
  float w = 0.0f; // Padding for SIMD alignment

  // Constructors
  Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
  Vec3(float scalar) : x(scalar), y(scalar), z(scalar) {}
  Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
  Vec3(const Vec2 &xy, float z) : x(xy.x), y(xy.y), z(z) {}

  // Static factory methods
  static Vec3 Zero() { return Vec3(0.0f, 0.0f, 0.0f); }
  static Vec3 One() { return Vec3(1.0f, 1.0f, 1.0f); }
  static Vec3 UnitX() { return Vec3(1.0f, 0.0f, 0.0f); }
  static Vec3 UnitY() { return Vec3(0.0f, 1.0f, 0.0f); }
  static Vec3 UnitZ() { return Vec3(0.0f, 0.0f, 1.0f); }
  static Vec3 Forward() { return Vec3(0.0f, 0.0f, -1.0f); }
  static Vec3 Back() { return Vec3(0.0f, 0.0f, 1.0f); }
  static Vec3 Up() { return Vec3(0.0f, 1.0f, 0.0f); }
  static Vec3 Down() { return Vec3(0.0f, -1.0f, 0.0f); }
  static Vec3 Right() { return Vec3(1.0f, 0.0f, 0.0f); }
  static Vec3 Left() { return Vec3(-1.0f, 0.0f, 0.0f); }

  // Operators
  Vec3 operator+(const Vec3 &other) const {
    Vec3 result;
    __m128 a = _mm_load_ps(&x);
    __m128 b = _mm_load_ps(&other.x);
    __m128 res = _mm_add_ps(a, b);
    _mm_store_ps(&result.x, res);
    return result;
  }

  Vec3 operator-(const Vec3 &other) const {
    Vec3 result;
    __m128 a = _mm_load_ps(&x);
    __m128 b = _mm_load_ps(&other.x);
    __m128 res = _mm_sub_ps(a, b);
    _mm_store_ps(&result.x, res);
    return result;
  }

  Vec3 operator*(float scalar) const {
    Vec3 result;
    __m128 a = _mm_load_ps(&x);
    __m128 s = _mm_set1_ps(scalar);
    __m128 res = _mm_mul_ps(a, s);
    _mm_store_ps(&result.x, res);
    return result;
  }

  Vec3 operator*(const Vec3 &other) const {
    Vec3 result;
    __m128 a = _mm_load_ps(&x);
    __m128 b = _mm_load_ps(&other.x);
    __m128 res = _mm_mul_ps(a, b);
    _mm_store_ps(&result.x, res);
    return result;
  }

  Vec3 operator/(float scalar) const {
    Vec3 result;
    __m128 a = _mm_load_ps(&x);
    __m128 s = _mm_set1_ps(scalar);
    __m128 res = _mm_div_ps(a, s);
    _mm_store_ps(&result.x, res);
    return result;
  }

  Vec3 operator/(const Vec3 &other) const {
    Vec3 result;
    __m128 a = _mm_load_ps(&x);
    __m128 b = _mm_load_ps(&other.x);
    __m128 res = _mm_div_ps(a, b);
    _mm_store_ps(&result.x, res);
    return result;
  }

  Vec3 operator-() const { return Vec3(-x, -y, -z); }

  Vec3 &operator+=(const Vec3 &other) {
    __m128 a = _mm_load_ps(&x);
    __m128 b = _mm_load_ps(&other.x);
    __m128 res = _mm_add_ps(a, b);
    _mm_store_ps(&x, res);
    return *this;
  }

  Vec3 &operator-=(const Vec3 &other) {
    __m128 a = _mm_load_ps(&x);
    __m128 b = _mm_load_ps(&other.x);
    __m128 res = _mm_sub_ps(a, b);
    _mm_store_ps(&x, res);
    return *this;
  }

  Vec3 &operator*=(float scalar) {
    __m128 a = _mm_load_ps(&x);
    __m128 s = _mm_set1_ps(scalar);
    __m128 res = _mm_mul_ps(a, s);
    _mm_store_ps(&x, res);
    return *this;
  }

  Vec3 &operator/=(float scalar) {
    __m128 a = _mm_load_ps(&x);
    __m128 s = _mm_set1_ps(scalar);
    __m128 res = _mm_div_ps(a, s);
    _mm_store_ps(&x, res);
    return *this;
  }

  float &operator[](int index) { return data[index]; }
  const float &operator[](int index) const { return data[index]; }

  // Vector operations
  float Dot(const Vec3 &other) const {
    __m128 a = _mm_load_ps(&x);
    __m128 b = _mm_load_ps(&other.x);
    __m128 mul = _mm_mul_ps(a, b);
    __m128 hadd1 = _mm_hadd_ps(mul, mul);
    __m128 hadd2 = _mm_hadd_ps(hadd1, hadd1);
    return _mm_cvtss_f32(hadd2);
  }

  Vec3 Cross(const Vec3 &other) const {
    return Vec3(y * other.z - z * other.y, z * other.x - x * other.z,
                x * other.y - y * other.x);
  }

  float Length() const { return Sqrt(Dot(*this)); }

  float LengthSquared() const { return Dot(*this); }

  Vec3 Normalized() const {
    float len = Length();
    return IsNearZero(len) ? Vec3::Zero() : *this / len;
  }

  void Normalize() {
    float len = Length();
    if (!IsNearZero(len)) {
      *this /= len;
    }
  }

  Vec3 Lerp(const Vec3 &other, float t) const {
    return *this + (other - *this) * t;
  }

  Vec3 Reflect(const Vec3 &normal) const {
    return *this - normal * 2.0f * Dot(normal);
  }

  Vec2 XY() const { return Vec2(x, y); }
  Vec2 XZ() const { return Vec2(x, z); }
  Vec2 YZ() const { return Vec2(y, z); }
};

//============================================================================
// Vec4 - 4D Vector with SIMD optimization
//============================================================================
struct alignas(16) Vec4 {
  union {
    struct {
      float x, y, z, w;
    };
    float data[4];
    __m128 simd;
  };

  // Constructors
  Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
  Vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
  Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
  Vec4(const Vec3 &xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
  Vec4(const Vec2 &xy, const Vec2 &zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}

  // Static factory methods
  static Vec4 Zero() { return Vec4(0.0f, 0.0f, 0.0f, 0.0f); }
  static Vec4 One() { return Vec4(1.0f, 1.0f, 1.0f, 1.0f); }

  // Operators with SIMD
  Vec4 operator+(const Vec4 &other) const {
    Vec4 result;
    result.simd = _mm_add_ps(simd, other.simd);
    return result;
  }

  Vec4 operator-(const Vec4 &other) const {
    Vec4 result;
    result.simd = _mm_sub_ps(simd, other.simd);
    return result;
  }

  Vec4 operator*(float scalar) const {
    Vec4 result;
    __m128 s = _mm_set1_ps(scalar);
    result.simd = _mm_mul_ps(simd, s);
    return result;
  }

  Vec4 operator/(float scalar) const {
    Vec4 result;
    __m128 s = _mm_set1_ps(scalar);
    result.simd = _mm_div_ps(simd, s);
    return result;
  }

  Vec4 operator-() const {
    Vec4 result;
    __m128 zero = _mm_setzero_ps();
    result.simd = _mm_sub_ps(zero, simd);
    return result;
  }

  Vec4 &operator+=(const Vec4 &other) {
    simd = _mm_add_ps(simd, other.simd);
    return *this;
  }

  Vec4 &operator-=(const Vec4 &other) {
    simd = _mm_sub_ps(simd, other.simd);
    return *this;
  }

  Vec4 &operator*=(float scalar) {
    __m128 s = _mm_set1_ps(scalar);
    simd = _mm_mul_ps(simd, s);
    return *this;
  }

  Vec4 &operator/=(float scalar) {
    __m128 s = _mm_set1_ps(scalar);
    simd = _mm_div_ps(simd, s);
    return *this;
  }

  float &operator[](int index) { return data[index]; }
  const float &operator[](int index) const { return data[index]; }

  // Vector operations
  float Dot(const Vec4 &other) const {
    __m128 mul = _mm_mul_ps(simd, other.simd);
    __m128 hadd1 = _mm_hadd_ps(mul, mul);
    __m128 hadd2 = _mm_hadd_ps(hadd1, hadd1);
    return _mm_cvtss_f32(hadd2);
  }

  float Length() const { return Sqrt(Dot(*this)); }

  float LengthSquared() const { return Dot(*this); }

  Vec4 Normalized() const {
    float len = Length();
    return IsNearZero(len) ? Vec4::Zero() : *this / len;
  }

  void Normalize() {
    float len = Length();
    if (!IsNearZero(len)) {
      *this /= len;
    }
  }

  Vec3 XYZ() const { return Vec3(x, y, z); }
  Vec2 XY() const { return Vec2(x, y); }
};

// Global operator overloads
inline Vec2 operator*(float scalar, const Vec2 &vec) { return vec * scalar; }
inline Vec3 operator*(float scalar, const Vec3 &vec) { return vec * scalar; }
inline Vec4 operator*(float scalar, const Vec4 &vec) { return vec * scalar; }

// Stream operators for debugging
inline std::ostream &operator<<(std::ostream &os, const Vec2 &v) {
  return os << "Vec2(" << v.x << ", " << v.y << ")";
}

inline std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
  return os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
}

inline std::ostream &operator<<(std::ostream &os, const Vec4 &v) {
  return os << "Vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
            << ")";
}

} // namespace Math
} // namespace Engine