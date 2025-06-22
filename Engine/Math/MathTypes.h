#pragma once

#include <cmath>
#include <limits>

namespace Engine {
namespace Math {

// Math constants
static constexpr float PI = 3.14159265358979323846f;
static constexpr float TWO_PI = 2.0f * PI;
static constexpr float HALF_PI = PI * 0.5f;
static constexpr float QUARTER_PI = PI * 0.25f;

static constexpr float EPSILON = 1e-6f;
static constexpr float FLOAT_EPSILON = std::numeric_limits<float>::epsilon();

static constexpr float DEG_TO_RAD = PI / 180.0f;
static constexpr float RAD_TO_DEG = 180.0f / PI;

// Utility functions
inline float ToRadians(float degrees) { return degrees * DEG_TO_RAD; }

inline float ToDegrees(float radians) { return radians * RAD_TO_DEG; }

inline bool IsNearZero(float value, float epsilon = EPSILON) {
  return std::abs(value) < epsilon;
}

inline bool IsEqual(float a, float b, float epsilon = EPSILON) {
  return std::abs(a - b) < epsilon;
}

inline float Clamp(float value, float min, float max) {
  return value < min ? min : (value > max ? max : value);
}

inline float Lerp(float a, float b, float t) { return a + t * (b - a); }

inline float Smoothstep(float edge0, float edge1, float x) {
  float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  return t * t * (3.0f - 2.0f * t);
}

template <typename T> inline T Min(T a, T b) { return a < b ? a : b; }

template <typename T> inline T Max(T a, T b) { return a > b ? a : b; }

template <typename T> inline T Abs(T value) {
  return value < 0 ? -value : value;
}

inline float Sqrt(float value) { return std::sqrt(value); }

inline float InvSqrt(float value) { return 1.0f / std::sqrt(value); }

inline float Sin(float angle) { return std::sin(angle); }

inline float Cos(float angle) { return std::cos(angle); }

inline float Tan(float angle) { return std::tan(angle); }

inline float Asin(float value) { return std::asin(value); }

inline float Acos(float value) { return std::acos(value); }

inline float Atan2(float y, float x) { return std::atan2(y, x); }

} // namespace Math
} // namespace Engine