#include "Core/Logger.h"
#include "Math/MathTypes.h"
#include <cassert>
#include <iostream>

using namespace Engine;

//============================================================================
// Test Helper Functions
//============================================================================
bool IsNearlyEqual(float a, float b, float epsilon = 1e-5f) {
  return std::abs(a - b) < epsilon;
}

#define TEST_ASSERT(condition, message)                                        \
  if (!(condition)) {                                                          \
    Logger::Error("Phase2Tests", std::string("FAILED: ") + message);           \
    return false;                                                              \
  }

#define TEST_NEARLY_EQUAL(a, b, message)                                       \
  if (!IsNearlyEqual(a, b)) {                                                  \
    Logger::Error("Phase2Tests", std::string("FAILED: ") + message);           \
    return false;                                                              \
  }

//============================================================================
// Core Math Constants Tests
//============================================================================
bool TestMathConstants() {
  Logger::Info("Phase2Tests", "Testing math constants...");

  // Test PI constant
  TEST_NEARLY_EQUAL(Math::PI, 3.14159265f, "PI constant");
  TEST_NEARLY_EQUAL(Math::TWO_PI, 2.0f * Math::PI, "TWO_PI constant");
  TEST_NEARLY_EQUAL(Math::HALF_PI, Math::PI * 0.5f, "HALF_PI constant");

  // Test conversion functions
  float degrees90 = Math::ToDegrees(Math::HALF_PI);
  TEST_NEARLY_EQUAL(degrees90, 90.0f, "Radians to degrees conversion");

  float radians90 = Math::ToRadians(90.0f);
  TEST_NEARLY_EQUAL(radians90, Math::HALF_PI, "Degrees to radians conversion");

  // Test utility functions
  TEST_NEARLY_EQUAL(Math::Clamp(5.0f, 0.0f, 10.0f), 5.0f, "Clamp within range");
  TEST_NEARLY_EQUAL(Math::Clamp(-1.0f, 0.0f, 10.0f), 0.0f, "Clamp below range");
  TEST_NEARLY_EQUAL(Math::Clamp(15.0f, 0.0f, 10.0f), 10.0f,
                    "Clamp above range");

  TEST_NEARLY_EQUAL(Math::Lerp(0.0f, 10.0f, 0.5f), 5.0f,
                    "Linear interpolation");

  Logger::Info("Phase2Tests", "✅ Math constants tests passed!");
  return true;
}

//============================================================================
// Trigonometry Tests
//============================================================================
bool TestTrigonometry() {
  Logger::Info("Phase2Tests", "Testing trigonometry functions...");

  // Test basic trig functions
  TEST_NEARLY_EQUAL(Math::Sin(0.0f), 0.0f, "Sin(0)");
  TEST_NEARLY_EQUAL(Math::Sin(Math::HALF_PI), 1.0f, "Sin(PI/2)");

  TEST_NEARLY_EQUAL(Math::Cos(0.0f), 1.0f, "Cos(0)");
  TEST_NEARLY_EQUAL(Math::Cos(Math::HALF_PI), 0.0f, "Cos(PI/2)");

  TEST_NEARLY_EQUAL(Math::Tan(0.0f), 0.0f, "Tan(0)");

  // Test square root
  TEST_NEARLY_EQUAL(Math::Sqrt(16.0f), 4.0f, "Square root");
  TEST_NEARLY_EQUAL(Math::InvSqrt(0.25f), 2.0f, "Inverse square root");

  Logger::Info("Phase2Tests", "✅ Trigonometry tests passed!");
  return true;
}

//============================================================================
// Template and Utility Tests
//============================================================================
bool TestUtilities() {
  Logger::Info("Phase2Tests", "Testing utility templates...");

  // Test Min/Max templates
  TEST_ASSERT(Math::Min(5, 10) == 5, "Min template");
  TEST_ASSERT(Math::Max(5, 10) == 10, "Max template");
  TEST_ASSERT(Math::Abs(-5) == 5, "Abs template");

  // Test floating point comparisons
  TEST_ASSERT(Math::IsNearZero(0.000001f, 0.00001f), "IsNearZero small value");
  TEST_ASSERT(!Math::IsNearZero(1.0f), "IsNearZero large value");

  TEST_ASSERT(Math::IsEqual(1.0f, 1.0001f, 0.001f), "IsEqual with tolerance");
  TEST_ASSERT(!Math::IsEqual(1.0f, 1.1f, 0.001f), "IsEqual different values");

  Logger::Info("Phase2Tests", "✅ Utility tests passed!");
  return true;
}

//============================================================================
// Performance Benchmark
//============================================================================
bool TestPerformance() {
  Logger::Info("Phase2Tests", "Testing performance...");

  const int iterations = 10000;

  // Simple performance test - just verify functions work in bulk
  float sum = 0.0f;
  for (int i = 0; i < iterations; ++i) {
    float angle = static_cast<float>(i) * 0.001f;
    sum += Math::Sin(angle) + Math::Cos(angle);
  }

  TEST_ASSERT(sum != 0.0f, "Performance test produces non-zero result");

  Logger::Info("Phase2Tests", "✅ Performance test completed!");
  return true;
}

//============================================================================
// Main Test Runner
//============================================================================
int main() {
  Logger::Info("Phase2Tests", "🧮 Starting Phase 2 Math Foundation Tests...");
  Logger::Info("Phase2Tests",
               "Testing core math constants, trigonometry, and utilities");

  bool allPassed = true;

  // Run all test categories
  allPassed &= TestMathConstants();
  allPassed &= TestTrigonometry();
  allPassed &= TestUtilities();
  allPassed &= TestPerformance();

  if (allPassed) {
    Logger::Info("Phase2Tests", "🎉 ALL PHASE 2 CORE TESTS PASSED!");
    Logger::Info("Phase2Tests", "✅ Math constants validated");
    Logger::Info("Phase2Tests", "✅ Trigonometry functions validated");
    Logger::Info("Phase2Tests", "✅ Utility functions validated");
    Logger::Info("Phase2Tests", "✅ Performance tests completed");
    Logger::Info("Phase2Tests", "🚀 Phase 2 math foundation is solid!");
    Logger::Info("Phase2Tests",
                 "📐 Ready for SIMD vectors, matrices, and 3D systems!");
    return 0;
  } else {
    Logger::Error("Phase2Tests",
                  "❌ Some tests failed! Check the implementation.");
    return -1;
  }
}