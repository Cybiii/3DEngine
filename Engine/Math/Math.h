#pragma once

// Core math library for the 3D Engine
// Includes SIMD-optimized vectors, matrices, and quaternions

#include "MathTypes.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Vector.h"


namespace Engine {
namespace Math {

//============================================================================
// Geometric primitives and collision detection
//============================================================================

// Axis-Aligned Bounding Box
struct AABB {
  Vec3 min, max;

  AABB() : min(Vec3::Zero()), max(Vec3::Zero()) {}
  AABB(const Vec3 &min, const Vec3 &max) : min(min), max(max) {}

  Vec3 Center() const { return (min + max) * 0.5f; }
  Vec3 Size() const { return max - min; }
  Vec3 Extents() const { return Size() * 0.5f; }

  bool Contains(const Vec3 &point) const {
    return point.x >= min.x && point.x <= max.x && point.y >= min.y &&
           point.y <= max.y && point.z >= min.z && point.z <= max.z;
  }

  bool Intersects(const AABB &other) const {
    return min.x <= other.max.x && max.x >= other.min.x &&
           min.y <= other.max.y && max.y >= other.min.y &&
           min.z <= other.max.z && max.z >= other.min.z;
  }

  AABB Transformed(const Mat4 &transform) const {
    Vec3 corners[8] = {Vec3(min.x, min.y, min.z), Vec3(max.x, min.y, min.z),
                       Vec3(min.x, max.y, min.z), Vec3(max.x, max.y, min.z),
                       Vec3(min.x, min.y, max.z), Vec3(max.x, min.y, max.z),
                       Vec3(min.x, max.y, max.z), Vec3(max.x, max.y, max.z)};

    AABB result;
    result.min = result.max = transform.TransformPoint(corners[0]);

    for (int i = 1; i < 8; ++i) {
      Vec3 transformed = transform.TransformPoint(corners[i]);
      result.min = Vec3(Min(result.min.x, transformed.x),
                        Min(result.min.y, transformed.y),
                        Min(result.min.z, transformed.z));
      result.max = Vec3(Max(result.max.x, transformed.x),
                        Max(result.max.y, transformed.y),
                        Max(result.max.z, transformed.z));
    }

    return result;
  }
};

// Sphere
struct Sphere {
  Vec3 center;
  float radius;

  Sphere() : center(Vec3::Zero()), radius(0.0f) {}
  Sphere(const Vec3 &center, float radius) : center(center), radius(radius) {}

  bool Contains(const Vec3 &point) const {
    return (point - center).LengthSquared() <= radius * radius;
  }

  bool Intersects(const Sphere &other) const {
    float distance = (center - other.center).Length();
    return distance <= (radius + other.radius);
  }

  bool Intersects(const AABB &aabb) const {
    Vec3 closest = Vec3(Clamp(center.x, aabb.min.x, aabb.max.x),
                        Clamp(center.y, aabb.min.y, aabb.max.y),
                        Clamp(center.z, aabb.min.z, aabb.max.z));
    return (center - closest).LengthSquared() <= radius * radius;
  }
};

// Frustum for camera culling
struct Frustum {
  Vec4 planes[6]; // Left, Right, Bottom, Top, Near, Far

  Frustum() = default;

  static Frustum FromMatrix(const Mat4 &matrix) {
    Frustum frustum;

    // Extract frustum planes from projection * view matrix
    // Left plane
    frustum.planes[0] =
        Vec4(matrix.m[0][3] + matrix.m[0][0], matrix.m[1][3] + matrix.m[1][0],
             matrix.m[2][3] + matrix.m[2][0], matrix.m[3][3] + matrix.m[3][0]);

    // Right plane
    frustum.planes[1] =
        Vec4(matrix.m[0][3] - matrix.m[0][0], matrix.m[1][3] - matrix.m[1][0],
             matrix.m[2][3] - matrix.m[2][0], matrix.m[3][3] - matrix.m[3][0]);

    // Bottom plane
    frustum.planes[2] =
        Vec4(matrix.m[0][3] + matrix.m[0][1], matrix.m[1][3] + matrix.m[1][1],
             matrix.m[2][3] + matrix.m[2][1], matrix.m[3][3] + matrix.m[3][1]);

    // Top plane
    frustum.planes[3] =
        Vec4(matrix.m[0][3] - matrix.m[0][1], matrix.m[1][3] - matrix.m[1][1],
             matrix.m[2][3] - matrix.m[2][1], matrix.m[3][3] - matrix.m[3][1]);

    // Near plane
    frustum.planes[4] =
        Vec4(matrix.m[0][3] + matrix.m[0][2], matrix.m[1][3] + matrix.m[1][2],
             matrix.m[2][3] + matrix.m[2][2], matrix.m[3][3] + matrix.m[3][2]);

    // Far plane
    frustum.planes[5] =
        Vec4(matrix.m[0][3] - matrix.m[0][2], matrix.m[1][3] - matrix.m[1][2],
             matrix.m[2][3] - matrix.m[2][2], matrix.m[3][3] - matrix.m[3][2]);

    // Normalize planes
    for (int i = 0; i < 6; ++i) {
      Vec3 normal = frustum.planes[i].XYZ();
      float length = normal.Length();
      if (length > EPSILON) {
        frustum.planes[i] = frustum.planes[i] / length;
      }
    }

    return frustum;
  }

  bool Contains(const Vec3 &point) const {
    for (int i = 0; i < 6; ++i) {
      if (planes[i].XYZ().Dot(point) + planes[i].w < 0.0f) {
        return false;
      }
    }
    return true;
  }

  bool Intersects(const Sphere &sphere) const {
    for (int i = 0; i < 6; ++i) {
      if (planes[i].XYZ().Dot(sphere.center) + planes[i].w < -sphere.radius) {
        return false;
      }
    }
    return true;
  }

  bool Intersects(const AABB &aabb) const {
    for (int i = 0; i < 6; ++i) {
      Vec3 normal = planes[i].XYZ();
      Vec3 positive = Vec3(normal.x >= 0.0f ? aabb.max.x : aabb.min.x,
                           normal.y >= 0.0f ? aabb.max.y : aabb.min.y,
                           normal.z >= 0.0f ? aabb.max.z : aabb.min.z);

      if (normal.Dot(positive) + planes[i].w < 0.0f) {
        return false;
      }
    }
    return true;
  }
};

//============================================================================
// Ray and intersection testing
//============================================================================
struct Ray {
  Vec3 origin;
  Vec3 direction;

  Ray() : origin(Vec3::Zero()), direction(Vec3::Forward()) {}
  Ray(const Vec3 &origin, const Vec3 &direction)
      : origin(origin), direction(direction.Normalized()) {}

  Vec3 At(float t) const { return origin + direction * t; }

  bool IntersectSphere(const Sphere &sphere, float &t) const {
    Vec3 oc = origin - sphere.center;
    float a = direction.Dot(direction);
    float b = 2.0f * oc.Dot(direction);
    float c = oc.Dot(oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) {
      return false;
    }

    float sqrt_discriminant = Sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);

    if (t1 > 0.0f) {
      t = t1;
      return true;
    } else if (t2 > 0.0f) {
      t = t2;
      return true;
    }

    return false;
  }

  bool IntersectAABB(const AABB &aabb, float &t) const {
    Vec3 invDir =
        Vec3(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);

    Vec3 t1 = (aabb.min - origin) * invDir;
    Vec3 t2 = (aabb.max - origin) * invDir;

    Vec3 tmin = Vec3(Min(t1.x, t2.x), Min(t1.y, t2.y), Min(t1.z, t2.z));
    Vec3 tmax = Vec3(Max(t1.x, t2.x), Max(t1.y, t2.y), Max(t1.z, t2.z));

    float tNear = Max(Max(tmin.x, tmin.y), tmin.z);
    float tFar = Min(Min(tmax.x, tmax.y), tmax.z);

    if (tNear > tFar || tFar < 0.0f) {
      return false;
    }

    t = tNear > 0.0f ? tNear : tFar;
    return true;
  }
};

//============================================================================
// Transform - Combines translation, rotation, and scale
//============================================================================
struct Transform {
  Vec3 position;
  Quaternion rotation;
  Vec3 scale;

  Transform()
      : position(Vec3::Zero()), rotation(Quaternion::Identity()),
        scale(Vec3::One()) {}

  Transform(const Vec3 &position,
            const Quaternion &rotation = Quaternion::Identity(),
            const Vec3 &scale = Vec3::One())
      : position(position), rotation(rotation), scale(scale) {}

  Mat4 ToMatrix() const {
    return Mat4::Translation(position) * rotation.ToMatrix() *
           Mat4::Scale(scale);
  }

  Vec3 TransformPoint(const Vec3 &point) const {
    return rotation.RotateVector(point * scale) + position;
  }

  Vec3 TransformVector(const Vec3 &vector) const {
    return rotation.RotateVector(vector * scale);
  }

  Vec3 InverseTransformPoint(const Vec3 &point) const {
    return rotation.Inverse().RotateVector(point - position) / scale;
  }

  Vec3 InverseTransformVector(const Vec3 &vector) const {
    return rotation.Inverse().RotateVector(vector) / scale;
  }

  Transform Inverse() const {
    Quaternion invRotation = rotation.Inverse();
    Vec3 invScale = Vec3(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
    Vec3 invPosition = invRotation.RotateVector(-position) * invScale;
    return Transform(invPosition, invRotation, invScale);
  }

  static Transform Lerp(const Transform &a, const Transform &b, float t) {
    return Transform(a.position.Lerp(b.position, t),
                     Quaternion::Slerp(a.rotation, b.rotation, t),
                     a.scale.Lerp(b.scale, t));
  }
};

} // namespace Math
} // namespace Engine

// Convenience typedefs to avoid Engine::Math:: prefix
using Vec2 = Engine::Math::Vec2;
using Vec3 = Engine::Math::Vec3;
using Vec4 = Engine::Math::Vec4;
using Mat4 = Engine::Math::Mat4;
using Quaternion = Engine::Math::Quaternion;
using Transform = Engine::Math::Transform;
using AABB = Engine::Math::AABB;
using Sphere = Engine::Math::Sphere;
using Frustum = Engine::Math::Frustum;
using Ray = Engine::Math::Ray;