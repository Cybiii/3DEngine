#pragma once

#include "Matrix.h"
#include "Vector.h"

namespace Engine {
namespace Math {

//============================================================================
// Quaternion - For smooth 3D rotations
//============================================================================
struct Quaternion {
  float x, y, z, w;

  // Constructors
  Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
  Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
  Quaternion(const Vec3 &xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

  // Static factory methods
  static Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

  static Quaternion FromAxisAngle(const Vec3 &axis, float angle) {
    Vec3 normalizedAxis = axis.Normalized();
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = Sin(halfAngle);
    return Quaternion(normalizedAxis.x * sinHalfAngle,
                      normalizedAxis.y * sinHalfAngle,
                      normalizedAxis.z * sinHalfAngle, Cos(halfAngle));
  }

  static Quaternion FromEulerAngles(float pitch, float yaw, float roll) {
    return FromEulerAngles(Vec3(pitch, yaw, roll));
  }

  static Quaternion FromEulerAngles(const Vec3 &euler) {
    float cx = Cos(euler.x * 0.5f);
    float sx = Sin(euler.x * 0.5f);
    float cy = Cos(euler.y * 0.5f);
    float sy = Sin(euler.y * 0.5f);
    float cz = Cos(euler.z * 0.5f);
    float sz = Sin(euler.z * 0.5f);

    return Quaternion(sx * cy * cz - cx * sy * sz, cx * sy * cz + sx * cy * sz,
                      cx * cy * sz - sx * sy * cz, cx * cy * cz + sx * sy * sz);
  }

  static Quaternion FromMatrix(const Mat4 &matrix) {
    float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];
    Quaternion result;

    if (trace > 0.0f) {
      float s = Sqrt(trace + 1.0f) * 2.0f;
      result.w = 0.25f * s;
      result.x = (matrix.m[2][1] - matrix.m[1][2]) / s;
      result.y = (matrix.m[0][2] - matrix.m[2][0]) / s;
      result.z = (matrix.m[1][0] - matrix.m[0][1]) / s;
    } else if (matrix.m[0][0] > matrix.m[1][1] &&
               matrix.m[0][0] > matrix.m[2][2]) {
      float s =
          Sqrt(1.0f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]) * 2.0f;
      result.w = (matrix.m[2][1] - matrix.m[1][2]) / s;
      result.x = 0.25f * s;
      result.y = (matrix.m[0][1] + matrix.m[1][0]) / s;
      result.z = (matrix.m[0][2] + matrix.m[2][0]) / s;
    } else if (matrix.m[1][1] > matrix.m[2][2]) {
      float s =
          Sqrt(1.0f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2]) * 2.0f;
      result.w = (matrix.m[0][2] - matrix.m[2][0]) / s;
      result.x = (matrix.m[0][1] + matrix.m[1][0]) / s;
      result.y = 0.25f * s;
      result.z = (matrix.m[1][2] + matrix.m[2][1]) / s;
    } else {
      float s =
          Sqrt(1.0f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]) * 2.0f;
      result.w = (matrix.m[1][0] - matrix.m[0][1]) / s;
      result.x = (matrix.m[0][2] + matrix.m[2][0]) / s;
      result.y = (matrix.m[1][2] + matrix.m[2][1]) / s;
      result.z = 0.25f * s;
    }

    return result;
  }

  // Operators
  Quaternion operator+(const Quaternion &other) const {
    return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
  }

  Quaternion operator-(const Quaternion &other) const {
    return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
  }

  Quaternion operator*(float scalar) const {
    return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
  }

  Quaternion operator*(const Quaternion &other) const {
    return Quaternion(w * other.x + x * other.w + y * other.z - z * other.y,
                      w * other.y - x * other.z + y * other.w + z * other.x,
                      w * other.z + x * other.y - y * other.x + z * other.w,
                      w * other.w - x * other.x - y * other.y - z * other.z);
  }

  Quaternion operator-() const { return Quaternion(-x, -y, -z, -w); }

  Quaternion &operator+=(const Quaternion &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  Quaternion &operator-=(const Quaternion &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  Quaternion &operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  Quaternion &operator*=(const Quaternion &other) {
    *this = *this * other;
    return *this;
  }

  // Quaternion operations
  float Dot(const Quaternion &other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
  }

  float Length() const { return Sqrt(x * x + y * y + z * z + w * w); }

  float LengthSquared() const { return x * x + y * y + z * z + w * w; }

  Quaternion Normalized() const {
    float len = Length();
    return IsNearZero(len) ? Identity() : *this * (1.0f / len);
  }

  void Normalize() {
    float len = Length();
    if (!IsNearZero(len)) {
      float invLen = 1.0f / len;
      x *= invLen;
      y *= invLen;
      z *= invLen;
      w *= invLen;
    }
  }

  Quaternion Conjugate() const { return Quaternion(-x, -y, -z, w); }

  Quaternion Inverse() const {
    float lenSq = LengthSquared();
    if (IsNearZero(lenSq)) {
      return Identity();
    }
    return Conjugate() * (1.0f / lenSq);
  }

  // Rotation operations
  Vec3 RotateVector(const Vec3 &vector) const {
    Quaternion qvec(vector.x, vector.y, vector.z, 0.0f);
    Quaternion result = *this * qvec * Conjugate();
    return Vec3(result.x, result.y, result.z);
  }

  // Conversions
  Vec3 ToEulerAngles() const {
    Vec3 euler;

    // Roll (x-axis rotation)
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    euler.x = Atan2(sinr_cosp, cosr_cosp);

    // Pitch (y-axis rotation)
    float sinp = 2.0f * (w * y - z * x);
    if (Abs(sinp) >= 1.0f) {
      euler.y = (sinp >= 0.0f) ? HALF_PI : -HALF_PI;
    } else {
      euler.y = Asin(sinp);
    }

    // Yaw (z-axis rotation)
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    euler.z = Atan2(siny_cosp, cosy_cosp);

    return euler;
  }

  Mat4 ToMatrix() const {
    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    Mat4 result = Mat4::Identity();
    result.m[0][0] = 1.0f - 2.0f * (yy + zz);
    result.m[0][1] = 2.0f * (xy - wz);
    result.m[0][2] = 2.0f * (xz + wy);

    result.m[1][0] = 2.0f * (xy + wz);
    result.m[1][1] = 1.0f - 2.0f * (xx + zz);
    result.m[1][2] = 2.0f * (yz - wx);

    result.m[2][0] = 2.0f * (xz - wy);
    result.m[2][1] = 2.0f * (yz + wx);
    result.m[2][2] = 1.0f - 2.0f * (xx + yy);

    return result;
  }

  void ToAxisAngle(Vec3 &axis, float &angle) const {
    if (Abs(w) > 1.0f) {
      Quaternion normalized = Normalized();
      normalized.ToAxisAngle(axis, angle);
      return;
    }

    angle = 2.0f * Acos(w);
    float s = Sqrt(1.0f - w * w);

    if (s < EPSILON) {
      axis = Vec3::UnitX();
    } else {
      axis = Vec3(x / s, y / s, z / s);
    }
  }

  // Interpolation
  static Quaternion Lerp(const Quaternion &a, const Quaternion &b, float t) {
    float dot = a.Dot(b);

    // Choose the shorter rotation path
    Quaternion result;
    if (dot < 0.0f) {
      result = a + ((-b - a) * t);
    } else {
      result = a + ((b - a) * t);
    }

    return result.Normalized();
  }

  static Quaternion Slerp(const Quaternion &a, const Quaternion &b, float t) {
    float dot = a.Dot(b);

    // Choose the shorter rotation path
    Quaternion b_corrected = (dot < 0.0f) ? -b : b;
    dot = Abs(dot);

    // If the quaternions are very close, use linear interpolation
    if (dot > 0.9995f) {
      return Lerp(a, b_corrected, t);
    }

    float theta = Acos(dot);
    float sinTheta = Sin(theta);
    float wa = Sin((1.0f - t) * theta) / sinTheta;
    float wb = Sin(t * theta) / sinTheta;

    return (a * wa + b_corrected * wb).Normalized();
  }

  // Utility functions
  Vec3 Forward() const { return RotateVector(Vec3::Forward()); }

  Vec3 Right() const { return RotateVector(Vec3::Right()); }

  Vec3 Up() const { return RotateVector(Vec3::Up()); }
};

// Global operators
inline Quaternion operator*(float scalar, const Quaternion &quat) {
  return quat * scalar;
}

// Stream operator for debugging
inline std::ostream &operator<<(std::ostream &os, const Quaternion &q) {
  return os << "Quaternion(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w
            << ")";
}

} // namespace Math
} // namespace Engine