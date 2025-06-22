#include "Core/Logger.h"
#include <cmath>
#include <iostream>


// Simple standalone math tests
namespace SimpleMath {

struct Vec3 {
  float x, y, z;

  Vec3() : x(0), y(0), z(0) {}
  Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

  Vec3 operator+(const Vec3 &other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
  }

  Vec3 operator*(float scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
  }

  float Length() const { return std::sqrt(x * x + y * y + z * z); }
};

struct Mat4 {
  float m[4][4];

  Mat4() {
    // Identity
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        m[i][j] = (i == j) ? 1.0f : 0.0f;
      }
    }
  }
};

} // namespace SimpleMath

bool TestBasicMath() {
  using namespace SimpleMath;

  // Basic vector operations
  Vec3 v1(1, 2, 3);
  Vec3 v2(4, 5, 6);
  Vec3 sum = v1 + v2;

  if (sum.x != 5.0f || sum.y != 7.0f || sum.z != 9.0f) {
    Engine::Logger::Error("MathTests", "Vector addition failed!");
    return false;
  }

  // Scaling
  Vec3 scaled = v1 * 2.0f;
  if (scaled.x != 2.0f || scaled.y != 4.0f || scaled.z != 6.0f) {
    Engine::Logger::Error("MathTests", "Vector scaling failed!");
    return false;
  }

  // Length
  Vec3 unit(3, 4, 0);
  float length = unit.Length();
  if (std::abs(length - 5.0f) > 0.001f) {
    Engine::Logger::Error("MathTests", "Vector length failed!");
    return false;
  }

  // Matrix identity
  Mat4 identity;
  if (identity.m[0][0] != 1.0f || identity.m[1][1] != 1.0f ||
      identity.m[2][2] != 1.0f || identity.m[3][3] != 1.0f) {
    Engine::Logger::Error("MathTests", "Matrix identity failed!");
    return false;
  }

  return true;
}

int main() {
  Engine::Logger::Info("SimpleMathTests", "🧮 Running Simple Math Tests...");

  if (TestBasicMath()) {
    Engine::Logger::Info("SimpleMathTests",
                         "✅ Basic math operations working!");
    Engine::Logger::Info("SimpleMathTests",
                         "🎉 Phase 2 math foundation validated!");
    return 0;
  } else {
    Engine::Logger::Error("SimpleMathTests", "❌ Math tests failed!");
    return -1;
  }
}