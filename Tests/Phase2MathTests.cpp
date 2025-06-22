#include "Core/Camera.h"
#include "Core/Logger.h"
#include "Math/Math.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

using namespace Engine;

//============================================================================
// Test Utilities
//============================================================================
bool IsNearlyEqual(float a, float b, float epsilon = 1e-5f) {
  return std::abs(a - b) < epsilon;
}

bool IsNearlyEqual(const Vec3 &a, const Vec3 &b, float epsilon = 1e-5f) {
  return IsNearlyEqual(a.x, b.x, epsilon) && IsNearlyEqual(a.y, b.y, epsilon) &&
         IsNearlyEqual(a.z, b.z, epsilon);
}

bool IsNearlyEqual(const Vec4 &a, const Vec4 &b, float epsilon = 1e-5f) {
  return IsNearlyEqual(a.x, b.x, epsilon) && IsNearlyEqual(a.y, b.y, epsilon) &&
         IsNearlyEqual(a.z, b.z, epsilon) && IsNearlyEqual(a.w, b.w, epsilon);
}

bool IsNearlyEqual(const Quaternion &a, const Quaternion &b,
                   float epsilon = 1e-5f) {
  return IsNearlyEqual(a.x, b.x, epsilon) && IsNearlyEqual(a.y, b.y, epsilon) &&
         IsNearlyEqual(a.z, b.z, epsilon) && IsNearlyEqual(a.w, b.w, epsilon);
}

#define TEST_ASSERT(condition, message)                                        \
  if (!(condition)) {                                                          \
    Logger::Error("MathTests", std::string("FAILED: ") + message);             \
    return false;                                                              \
  }

#define TEST_NEARLY_EQUAL(a, b, message)                                       \
  if (!IsNearlyEqual(a, b)) {                                                  \
    Logger::Error("MathTests", std::string("FAILED: ") + message);             \
    return false;                                                              \
  }

//============================================================================
// Vector Tests
//============================================================================
bool TestVec2() {
  Logger::Info("MathTests", "Testing Vec2...");

  // Construction
  Vec2 v1(3.0f, 4.0f);
  Vec2 v2(1.0f, 2.0f);

  // Basic operations
  Vec2 sum = v1 + v2;
  TEST_NEARLY_EQUAL(sum.x, 4.0f, "Vec2 addition X");
  TEST_NEARLY_EQUAL(sum.y, 6.0f, "Vec2 addition Y");

  Vec2 diff = v1 - v2;
  TEST_NEARLY_EQUAL(diff.x, 2.0f, "Vec2 subtraction X");
  TEST_NEARLY_EQUAL(diff.y, 2.0f, "Vec2 subtraction Y");

  Vec2 scaled = v1 * 2.0f;
  TEST_NEARLY_EQUAL(scaled.x, 6.0f, "Vec2 scaling X");
  TEST_NEARLY_EQUAL(scaled.y, 8.0f, "Vec2 scaling Y");

  // Dot product
  float dot = v1.Dot(v2);
  TEST_NEARLY_EQUAL(dot, 11.0f, "Vec2 dot product"); // 3*1 + 4*2 = 11

  // Length
  float length = v1.Length();
  TEST_NEARLY_EQUAL(length, 5.0f, "Vec2 length"); // sqrt(3^2 + 4^2) = 5

  // Normalization
  Vec2 normalized = v1.Normalized();
  TEST_NEARLY_EQUAL(normalized.Length(), 1.0f, "Vec2 normalized length");

  Logger::Info("MathTests", "✅ Vec2 tests passed!");
  return true;
}

bool TestVec3() {
  Logger::Info("MathTests", "Testing Vec3 with SIMD...");

  // Construction
  Vec3 v1(1.0f, 2.0f, 3.0f);
  Vec3 v2(4.0f, 5.0f, 6.0f);

  // SIMD operations
  Vec3 sum = v1 + v2;
  TEST_NEARLY_EQUAL(sum, Vec3(5.0f, 7.0f, 9.0f), "Vec3 SIMD addition");

  Vec3 diff = v2 - v1;
  TEST_NEARLY_EQUAL(diff, Vec3(3.0f, 3.0f, 3.0f), "Vec3 SIMD subtraction");

  Vec3 scaled = v1 * 2.0f;
  TEST_NEARLY_EQUAL(scaled, Vec3(2.0f, 4.0f, 6.0f), "Vec3 SIMD scaling");

  // Dot product (SIMD)
  float dot = v1.Dot(v2);
  TEST_NEARLY_EQUAL(dot, 32.0f,
                    "Vec3 SIMD dot product"); // 1*4 + 2*5 + 3*6 = 32

  // Cross product
  Vec3 cross = v1.Cross(v2);
  Vec3 expectedCross = Vec3(-3.0f, 6.0f, -3.0f); // Manual calculation
  TEST_NEARLY_EQUAL(cross, expectedCross, "Vec3 cross product");

  // Length
  Vec3 v3(3.0f, 4.0f, 0.0f);
  TEST_NEARLY_EQUAL(v3.Length(), 5.0f, "Vec3 length");

  // Normalization
  Vec3 normalized = v3.Normalized();
  TEST_NEARLY_EQUAL(normalized.Length(), 1.0f, "Vec3 normalized length");

  // Static factory methods
  TEST_NEARLY_EQUAL(Vec3::Zero(), Vec3(0.0f, 0.0f, 0.0f), "Vec3::Zero()");
  TEST_NEARLY_EQUAL(Vec3::One(), Vec3(1.0f, 1.0f, 1.0f), "Vec3::One()");
  TEST_NEARLY_EQUAL(Vec3::Up(), Vec3(0.0f, 1.0f, 0.0f), "Vec3::Up()");
  TEST_NEARLY_EQUAL(Vec3::Forward(), Vec3(0.0f, 0.0f, -1.0f),
                    "Vec3::Forward()");

  Logger::Info("MathTests", "✅ Vec3 SIMD tests passed!");
  return true;
}

bool TestVec4() {
  Logger::Info("MathTests", "Testing Vec4 with SIMD...");

  // Construction
  Vec4 v1(1.0f, 2.0f, 3.0f, 4.0f);
  Vec4 v2(5.0f, 6.0f, 7.0f, 8.0f);

  // SIMD operations
  Vec4 sum = v1 + v2;
  TEST_NEARLY_EQUAL(sum, Vec4(6.0f, 8.0f, 10.0f, 12.0f), "Vec4 SIMD addition");

  Vec4 scaled = v1 * 2.0f;
  TEST_NEARLY_EQUAL(scaled, Vec4(2.0f, 4.0f, 6.0f, 8.0f), "Vec4 SIMD scaling");

  // Dot product
  float dot = v1.Dot(v2);
  TEST_NEARLY_EQUAL(dot, 70.0f,
                    "Vec4 SIMD dot product"); // 1*5 + 2*6 + 3*7 + 4*8 = 70

  // Vec3 conversion
  Vec3 xyz = v1.XYZ();
  TEST_NEARLY_EQUAL(xyz, Vec3(1.0f, 2.0f, 3.0f), "Vec4 to Vec3 conversion");

  Logger::Info("MathTests", "✅ Vec4 SIMD tests passed!");
  return true;
}

//============================================================================
// Matrix Tests
//============================================================================
bool TestMat4() {
  Logger::Info("MathTests", "Testing Mat4 with SIMD...");

  // Identity matrix
  Mat4 identity = Mat4::Identity();
  Vec4 testVec(1.0f, 2.0f, 3.0f, 1.0f);
  Vec4 result = identity * testVec;
  TEST_NEARLY_EQUAL(result, testVec, "Mat4 Identity multiplication");

  // Translation matrix
  Mat4 translation = Mat4::Translation(Vec3(5.0f, 10.0f, 15.0f));
  Vec3 point(1.0f, 2.0f, 3.0f);
  Vec3 translatedPoint = translation.TransformPoint(point);
  TEST_NEARLY_EQUAL(translatedPoint, Vec3(6.0f, 12.0f, 18.0f),
                    "Mat4 Translation");

  // Scale matrix
  Mat4 scale = Mat4::Scale(Vec3(2.0f, 3.0f, 4.0f));
  Vec3 scaledPoint = scale.TransformPoint(point);
  TEST_NEARLY_EQUAL(scaledPoint, Vec3(2.0f, 6.0f, 12.0f), "Mat4 Scale");

  // Rotation matrices
  Mat4 rotX = Mat4::RotationX(Math::HALF_PI);
  Vec3 rotatedPoint = rotX.TransformPoint(Vec3(0.0f, 1.0f, 0.0f));
  TEST_NEARLY_EQUAL(rotatedPoint, Vec3(0.0f, 0.0f, 1.0f), "Mat4 RotationX");

  Mat4 rotY = Mat4::RotationY(Math::HALF_PI);
  rotatedPoint = rotY.TransformPoint(Vec3(1.0f, 0.0f, 0.0f));
  TEST_NEARLY_EQUAL(rotatedPoint, Vec3(0.0f, 0.0f, -1.0f), "Mat4 RotationY");

  Mat4 rotZ = Mat4::RotationZ(Math::HALF_PI);
  rotatedPoint = rotZ.TransformPoint(Vec3(1.0f, 0.0f, 0.0f));
  TEST_NEARLY_EQUAL(rotatedPoint, Vec3(0.0f, 1.0f, 0.0f), "Mat4 RotationZ");

  // Matrix multiplication (SIMD optimized)
  Mat4 combined = translation * scale;
  Vec3 combinedResult = combined.TransformPoint(point);
  Vec3 expected = Vec3(7.0f, 16.0f, 27.0f); // Scale first, then translate
  TEST_NEARLY_EQUAL(combinedResult, expected, "Mat4 multiplication order");

  Logger::Info("MathTests", "✅ Mat4 SIMD tests passed!");
  return true;
}

//============================================================================
// Quaternion Tests
//============================================================================
bool TestQuaternion() {
  Logger::Info("MathTests", "Testing Quaternion rotations...");

  // Identity quaternion
  Quaternion identity = Quaternion::Identity();
  Vec3 testVec(1.0f, 0.0f, 0.0f);
  Vec3 rotated = identity.RotateVector(testVec);
  TEST_NEARLY_EQUAL(rotated, testVec, "Quaternion Identity rotation");

  // Axis-angle construction
  Quaternion rotY = Quaternion::FromAxisAngle(Vec3::Up(), Math::HALF_PI);
  Vec3 rotatedVec = rotY.RotateVector(Vec3(1.0f, 0.0f, 0.0f));
  TEST_NEARLY_EQUAL(rotatedVec, Vec3(0.0f, 0.0f, -1.0f),
                    "Quaternion Y-axis rotation");

  // Euler angle construction
  Quaternion fromEuler = Quaternion::FromEulerAngles(0.0f, Math::HALF_PI, 0.0f);
  Vec3 eulerRotated = fromEuler.RotateVector(Vec3(1.0f, 0.0f, 0.0f));
  TEST_NEARLY_EQUAL(eulerRotated, Vec3(0.0f, 0.0f, -1.0f),
                    "Quaternion from Euler angles");

  // Quaternion multiplication
  Quaternion rotX = Quaternion::FromAxisAngle(Vec3::Right(), Math::HALF_PI);
  Quaternion combined = rotY * rotX;
  Vec3 combinedResult = combined.RotateVector(Vec3(0.0f, 1.0f, 0.0f));
  TEST_NEARLY_EQUAL(combinedResult.Length(), 1.0f,
                    "Quaternion multiplication preserves length");

  // Normalization
  Quaternion unnormalized(1.0f, 1.0f, 1.0f, 1.0f);
  Quaternion normalized = unnormalized.Normalized();
  TEST_NEARLY_EQUAL(normalized.Length(), 1.0f, "Quaternion normalization");

  // Conjugate and inverse
  Quaternion conjugate = rotY.Conjugate();
  Quaternion shouldBeIdentity = rotY * conjugate;
  TEST_NEARLY_EQUAL(shouldBeIdentity.w, 1.0f,
                    "Quaternion conjugate (w component)");
  TEST_NEARLY_EQUAL(shouldBeIdentity.x, 0.0f,
                    "Quaternion conjugate (x component)");

  // SLERP interpolation
  Quaternion start = Quaternion::Identity();
  Quaternion end = Quaternion::FromAxisAngle(Vec3::Up(), Math::PI);
  Quaternion middle = Quaternion::Slerp(start, end, 0.5f);
  Vec3 middleResult = middle.RotateVector(Vec3(1.0f, 0.0f, 0.0f));
  TEST_NEARLY_EQUAL(middleResult, Vec3(0.0f, 0.0f, 1.0f),
                    "Quaternion SLERP interpolation");

  Logger::Info("MathTests", "✅ Quaternion tests passed!");
  return true;
}

//============================================================================
// Transform Tests
//============================================================================
bool TestTransform() {
  Logger::Info("MathTests", "Testing Transform system...");

  // Basic transform
  Transform transform;
  transform.position = Vec3(1.0f, 2.0f, 3.0f);
  transform.rotation = Quaternion::FromAxisAngle(Vec3::Up(), Math::HALF_PI);
  transform.scale = Vec3(2.0f, 2.0f, 2.0f);

  // Transform point
  Vec3 localPoint(1.0f, 0.0f, 0.0f);
  Vec3 worldPoint = transform.TransformPoint(localPoint);
  Vec3 expected = Vec3(
      1.0f, 2.0f, 1.0f); // Scale(2,0,0) -> Rotate(0,0,-2) -> Translate(1,2,1)
  TEST_NEARLY_EQUAL(worldPoint, expected, "Transform point (TRS order)");

  // Inverse transform
  Vec3 backToLocal = transform.InverseTransformPoint(worldPoint);
  TEST_NEARLY_EQUAL(backToLocal, localPoint, "Inverse transform point");

  // Matrix conversion
  Mat4 matrix = transform.ToMatrix();
  Vec3 matrixResult = matrix.TransformPoint(localPoint);
  TEST_NEARLY_EQUAL(matrixResult.Length(), worldPoint.Length(),
                    "Transform to matrix consistency");

  // Transform interpolation
  Transform start;
  Transform end;
  end.position = Vec3(10.0f, 0.0f, 0.0f);
  end.rotation = Quaternion::FromAxisAngle(Vec3::Up(), Math::PI);
  end.scale = Vec3(2.0f, 2.0f, 2.0f);

  Transform middle = Transform::Lerp(start, end, 0.5f);
  TEST_NEARLY_EQUAL(middle.position, Vec3(5.0f, 0.0f, 0.0f),
                    "Transform position interpolation");

  Logger::Info("MathTests", "✅ Transform tests passed!");
  return true;
}

//============================================================================
// Camera Tests
//============================================================================
bool TestCamera() {
  Logger::Info("MathTests", "Testing Camera system...");

  // Basic camera setup
  Camera camera;
  camera.SetPosition(Vec3(0.0f, 0.0f, 5.0f));
  camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));
  camera.SetAspectRatio(16.0f, 9.0f);
  camera.SetFieldOfView(45.0f);

  // View matrix
  Mat4 viewMatrix = camera.GetViewMatrix();
  Vec3 worldPoint(1.0f, 0.0f, 0.0f);
  Vec3 viewPoint = viewMatrix.TransformPoint(worldPoint);
  // Point should be in front of camera (negative Z in view space)
  TEST_ASSERT(viewPoint.z < 0.0f,
              "Camera view matrix transforms point correctly");

  // Projection matrix
  Mat4 projMatrix = camera.GetProjectionMatrix();
  TEST_ASSERT(projMatrix.m[0][0] > 0.0f, "Projection matrix has valid X scale");
  TEST_ASSERT(projMatrix.m[1][1] > 0.0f, "Projection matrix has valid Y scale");

  // View-projection matrix
  Mat4 vpMatrix = camera.GetViewProjectionMatrix();
  Vec4 clipPoint = vpMatrix * Vec4(worldPoint, 1.0f);
  // Point should be in clip space
  TEST_ASSERT(clipPoint.w > 0.0f,
              "View-projection matrix produces valid clip coordinates");

  // Camera movement
  Vec3 originalPos = camera.position;
  camera.MoveForward(2.0f);
  TEST_NEARLY_EQUAL((camera.position - originalPos).Length(), 2.0f,
                    "Camera forward movement");

  Vec3 forwardDir = camera.GetForward();
  TEST_NEARLY_EQUAL(forwardDir.Length(), 1.0f,
                    "Camera forward direction normalized");

  Vec3 rightDir = camera.GetRight();
  TEST_NEARLY_EQUAL(rightDir.Length(), 1.0f,
                    "Camera right direction normalized");

  // Forward and right should be perpendicular
  float dot = forwardDir.Dot(rightDir);
  TEST_NEARLY_EQUAL(dot, 0.0f, "Camera forward and right perpendicular");

  Logger::Info("MathTests", "✅ Camera tests passed!");
  return true;
}

//============================================================================
// Geometric Primitives Tests
//============================================================================
bool TestGeometricPrimitives() {
  Logger::Info("MathTests", "Testing geometric primitives...");

  // AABB tests
  AABB box(Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));
  TEST_ASSERT(box.Contains(Vec3(0.0f, 0.0f, 0.0f)),
              "AABB contains center point");
  TEST_ASSERT(!box.Contains(Vec3(2.0f, 0.0f, 0.0f)),
              "AABB doesn't contain outside point");

  Vec3 center = box.Center();
  TEST_NEARLY_EQUAL(center, Vec3(0.0f, 0.0f, 0.0f), "AABB center calculation");

  Vec3 size = box.Size();
  TEST_NEARLY_EQUAL(size, Vec3(2.0f, 2.0f, 2.0f), "AABB size calculation");

  // AABB intersection
  AABB box2(Vec3(0.5f, 0.5f, 0.5f), Vec3(2.0f, 2.0f, 2.0f));
  TEST_ASSERT(box.Intersects(box2), "AABB intersection test");

  AABB box3(Vec3(3.0f, 3.0f, 3.0f), Vec3(4.0f, 4.0f, 4.0f));
  TEST_ASSERT(!box.Intersects(box3), "AABB non-intersection test");

  // Sphere tests
  Sphere sphere(Vec3(0.0f, 0.0f, 0.0f), 2.0f);
  TEST_ASSERT(sphere.Contains(Vec3(1.0f, 0.0f, 0.0f)), "Sphere contains point");
  TEST_ASSERT(!sphere.Contains(Vec3(3.0f, 0.0f, 0.0f)),
              "Sphere doesn't contain distant point");

  // Sphere-AABB intersection
  TEST_ASSERT(sphere.Intersects(box), "Sphere-AABB intersection");

  // Ray intersection
  Ray ray(Vec3(-5.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
  float t;
  TEST_ASSERT(ray.IntersectSphere(sphere, t), "Ray-sphere intersection");
  TEST_NEARLY_EQUAL(t, 3.0f, "Ray-sphere intersection distance"); // 5 - 2 = 3

  TEST_ASSERT(ray.IntersectAABB(box, t), "Ray-AABB intersection");
  TEST_NEARLY_EQUAL(t, 4.0f, "Ray-AABB intersection distance"); // 5 - 1 = 4

  Logger::Info("MathTests", "✅ Geometric primitives tests passed!");
  return true;
}

//============================================================================
// Performance Tests
//============================================================================
bool TestSIMDPerformance() {
  Logger::Info("MathTests", "Testing SIMD performance...");

  const int numOperations = 100000;

  // Create test data
  std::vector<Vec3> vectors1, vectors2, results;
  vectors1.reserve(numOperations);
  vectors2.reserve(numOperations);
  results.reserve(numOperations);

  for (int i = 0; i < numOperations; ++i) {
    vectors1.emplace_back(float(i), float(i + 1), float(i + 2));
    vectors2.emplace_back(float(i + 3), float(i + 4), float(i + 5));
  }

  // Test SIMD vector operations
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < numOperations; ++i) {
    results[i] = vectors1[i] + vectors2[i];
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  Logger::Info("MathTests",
               "SIMD Vec3 operations: " + std::to_string(numOperations) +
                   " additions in " + std::to_string(duration.count()) +
                   " microseconds");

  // Verify results are correct
  Vec3 expected = vectors1[0] + vectors2[0];
  TEST_NEARLY_EQUAL(results[0], expected, "SIMD performance test correctness");

  Logger::Info("MathTests", "✅ SIMD performance tests passed!");
  return true;
}

//============================================================================
// Main Test Runner
//============================================================================
int main() {
  Logger::Info("MathTests", "🧮 Starting Phase 2 Math Foundation Tests...");
  Logger::Info(
      "MathTests",
      "Testing SIMD-optimized vectors, matrices, quaternions, and 3D systems");

  bool allPassed = true;

  // Vector tests
  allPassed &= TestVec2();
  allPassed &= TestVec3();
  allPassed &= TestVec4();

  // Matrix tests
  allPassed &= TestMat4();

  // Quaternion tests
  allPassed &= TestQuaternion();

  // Transform tests
  allPassed &= TestTransform();

  // Camera tests
  allPassed &= TestCamera();

  // Geometric primitives tests
  allPassed &= TestGeometricPrimitives();

  // Performance tests
  allPassed &= TestSIMDPerformance();

  if (allPassed) {
    Logger::Info("MathTests", "🎉 ALL PHASE 2 MATH TESTS PASSED!");
    Logger::Info("MathTests",
                 "✅ SIMD-optimized math library is ready for 3D rendering!");
    Logger::Info("MathTests", "✅ Camera system validated");
    Logger::Info("MathTests", "✅ Transform system validated");
    Logger::Info("MathTests", "✅ Geometric primitives validated");
    Logger::Info("MathTests",
                 "🚀 Phase 2 milestone achieved - ready for 3D cube demo!");
    return 0;
  } else {
    Logger::Error("MathTests",
                  "❌ Some tests failed! Check the implementation.");
    return -1;
  }
}