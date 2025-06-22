#include "Core/Camera.h"
#include "Core/Logger.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include <cmath>
#include <iostream>


using namespace Engine;

// Test helper
bool TestEqual(float a, float b, float epsilon = 0.001f) {
  return std::abs(a - b) < epsilon;
}

bool TestVec3Equal(const Vec3 &a, const Vec3 &b, float epsilon = 0.001f) {
  return TestEqual(a.x, b.x, epsilon) && TestEqual(a.y, b.y, epsilon) &&
         TestEqual(a.z, b.z, epsilon);
}

bool TestMat4Equal(const Mat4 &a, const Mat4 &b, float epsilon = 0.001f) {
  for (int i = 0; i < 16; i++) {
    if (!TestEqual(a.data[i], b.data[i], epsilon)) {
      return false;
    }
  }
  return true;
}

bool TestQuaternionEqual(const Quaternion &a, const Quaternion &b,
                         float epsilon = 0.001f) {
  return TestEqual(a.x, b.x, epsilon) && TestEqual(a.y, b.y, epsilon) &&
         TestEqual(a.z, b.z, epsilon) && TestEqual(a.w, b.w, epsilon);
}

// Phase 2 Math Tests
bool TestVectorOperations() {
  std::cout << "Testing Vector Operations..." << std::endl;

  // Basic vector operations
  Vec3 a(1.0f, 2.0f, 3.0f);
  Vec3 b(4.0f, 5.0f, 6.0f);

  Vec3 sum = a + b;
  if (!TestVec3Equal(sum, Vec3(5.0f, 7.0f, 9.0f))) {
    std::cout << "  FAIL: Vector addition" << std::endl;
    return false;
  }

  float dot = a.Dot(b);
  if (!TestEqual(dot, 32.0f)) {
    std::cout << "  FAIL: Dot product. Expected: 32, Got: " << dot << std::endl;
    return false;
  }

  Vec3 cross = a.Cross(b);
  if (!TestVec3Equal(cross, Vec3(-3.0f, 6.0f, -3.0f))) {
    std::cout << "  FAIL: Cross product" << std::endl;
    return false;
  }

  float length = a.Length();
  if (!TestEqual(length, std::sqrt(14.0f))) {
    std::cout << "  FAIL: Vector length" << std::endl;
    return false;
  }

  std::cout << "  PASS: Vector operations" << std::endl;
  return true;
}

bool TestMatrixOperations() {
  std::cout << "Testing Matrix Operations..." << std::endl;

  // Identity matrix
  Mat4 identity = Mat4::Identity();
  Vec3 point(1.0f, 2.0f, 3.0f);
  Vec3 transformed = identity.TransformPoint(point);

  if (!TestVec3Equal(point, transformed)) {
    std::cout << "  FAIL: Identity matrix transformation" << std::endl;
    return false;
  }

  // Translation matrix
  Mat4 translation = Mat4::Translation(Vec3(5.0f, 10.0f, 15.0f));
  Vec3 translated = translation.TransformPoint(point);
  Vec3 expected(6.0f, 12.0f, 18.0f);

  if (!TestVec3Equal(translated, expected)) {
    std::cout << "  FAIL: Translation matrix" << std::endl;
    return false;
  }

  // Scale matrix
  Mat4 scale = Mat4::Scale(Vec3(2.0f, 3.0f, 4.0f));
  Vec3 scaled = scale.TransformPoint(point);
  Vec3 expectedScale(2.0f, 6.0f, 12.0f);

  if (!TestVec3Equal(scaled, expectedScale)) {
    std::cout << "  FAIL: Scale matrix" << std::endl;
    return false;
  }

  std::cout << "  PASS: Matrix operations" << std::endl;
  return true;
}

bool TestQuaternionOperations() {
  std::cout << "Testing Quaternion Operations..." << std::endl;

  // Identity quaternion
  Quaternion identity = Quaternion::Identity();
  if (!TestQuaternionEqual(identity, Quaternion(0, 0, 0, 1))) {
    std::cout << "  FAIL: Identity quaternion" << std::endl;
    return false;
  }

  // Rotation around Y axis (90 degrees)
  Quaternion yRot = Quaternion::FromAxisAngle(Vec3::Up(), Math::HALF_PI);
  Vec3 point(1.0f, 0.0f, 0.0f);
  Vec3 rotated = yRot.RotateVector(point);
  Vec3 expected(0.0f, 0.0f, -1.0f);

  if (!TestVec3Equal(rotated, expected, 0.01f)) {
    std::cout << "  FAIL: Quaternion rotation. Expected: (0,0,-1), Got: ("
              << rotated.x << "," << rotated.y << "," << rotated.z << ")"
              << std::endl;
    return false;
  }

  // Quaternion to matrix conversion
  Mat4 rotMatrix = yRot.ToMatrix();
  Vec3 matrixRotated = rotMatrix.TransformVector(point);

  if (!TestVec3Equal(rotated, matrixRotated, 0.01f)) {
    std::cout << "  FAIL: Quaternion to matrix conversion" << std::endl;
    return false;
  }

  std::cout << "  PASS: Quaternion operations" << std::endl;
  return true;
}

bool TestTransformOperations() {
  std::cout << "Testing Transform Operations..." << std::endl;

  Transform transform;
  transform.position = Vec3(1.0f, 2.0f, 3.0f);
  transform.rotation = Quaternion::FromAxisAngle(Vec3::Up(), Math::HALF_PI);
  transform.scale = Vec3(2.0f, 2.0f, 2.0f);

  Mat4 matrix = transform.ToMatrix();

  // Test that the transform matrix correctly combines TRS
  Vec3 point(1.0f, 0.0f, 0.0f);
  Vec3 transformed = matrix.TransformPoint(point);

  // Expected: Scale(2,2,2) * RotateY(90°) * Translate(1,2,3) * (1,0,0)
  // First scale: (2,0,0)
  // Then rotate: (0,0,-2)
  // Then translate: (1,2,1)
  Vec3 expected(1.0f, 2.0f, 1.0f);

  if (!TestVec3Equal(transformed, expected, 0.01f)) {
    std::cout << "  FAIL: Transform TRS composition. Expected: (" << expected.x
              << "," << expected.y << "," << expected.z << "), Got: ("
              << transformed.x << "," << transformed.y << "," << transformed.z
              << ")" << std::endl;
    return false;
  }

  std::cout << "  PASS: Transform operations" << std::endl;
  return true;
}

bool TestCameraOperations() {
  std::cout << "Testing Camera Operations..." << std::endl;

  Camera camera;
  camera.SetPosition(Vec3(0.0f, 0.0f, 5.0f));
  camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));
  camera.SetFieldOfView(45.0f);
  camera.SetAspectRatio(16.0f, 9.0f);

  // Test view matrix
  Mat4 view = camera.GetViewMatrix();

  // Test projection matrix
  Mat4 projection = camera.GetProjectionMatrix();

  // Test view-projection matrix
  Mat4 viewProjection = camera.GetViewProjectionMatrix();
  Mat4 expected = projection * view;

  if (!TestMat4Equal(viewProjection, expected)) {
    std::cout << "  FAIL: View-projection matrix composition" << std::endl;
    return false;
  }

  std::cout << "  PASS: Camera operations" << std::endl;
  return true;
}

bool TestRendererInitialization() {
  std::cout << "Testing Renderer Initialization..." << std::endl;

  if (!Renderer::Initialize()) {
    std::cout << "  FAIL: Renderer initialization" << std::endl;
    return false;
  }

  std::cout << "  PASS: Renderer initialization" << std::endl;
  return true;
}

int main() {
  std::cout << "=== Phase 2 Integration Tests ===" << std::endl;
  std::cout << "Testing 3D Math Library and Basic Rendering" << std::endl;
  std::cout << std::endl;

  int passed = 0;
  int total = 0;

  // Initialize logger for renderer tests
  Logger::Initialize();

  // Math tests
  total++;
  if (TestVectorOperations())
    passed++;
  total++;
  if (TestMatrixOperations())
    passed++;
  total++;
  if (TestQuaternionOperations())
    passed++;
  total++;
  if (TestTransformOperations())
    passed++;
  total++;
  if (TestCameraOperations())
    passed++;

  // Renderer test (requires OpenGL context)
  std::cout << "Note: Renderer tests require OpenGL context (window creation)"
            << std::endl;

  std::cout << std::endl;
  std::cout << "=== Results ===" << std::endl;
  std::cout << "Tests passed: " << passed << "/" << total << std::endl;

  if (passed == total) {
    std::cout << "🎉 All Phase 2 tests PASSED!" << std::endl;
    std::cout << "✅ 3D Math library is working correctly" << std::endl;
    std::cout << "✅ Vectors, Matrices, Quaternions, Transforms all functional"
              << std::endl;
    std::cout << "✅ Camera system operational" << std::endl;
    std::cout << "✅ Ready for Phase 3 development!" << std::endl;
    return 0;
  } else {
    std::cout << "❌ Some tests failed. Phase 2 needs fixes." << std::endl;
    return 1;
  }
}