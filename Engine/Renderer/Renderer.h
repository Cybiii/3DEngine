#pragma once

#include "Core/Logger.h"
#include "Math/Math.h"
#include <memory>

namespace Engine {

// Forward declarations
class Shader;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Buffer;
class Camera;

class Renderer {
public:
  // Core renderer methods
  static bool Initialize();
  static void Shutdown();

  // Rendering operations
  static void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f,
                    float a = 1.0f);
  static void SetViewport(int x, int y, int width, int height);

  // 2D Triangle rendering (Phase 1)
  static void DrawTriangle();

  // Animated effects (Phase 1)
  static void DrawAnimatedTriangle(float time);
  static void DrawTriangleSpiral(float time, int count = 12);
  static void DrawColorCyclingTriangles(float time);
  static void DrawMorphingShape(float time);

  // 3D Cube rendering (Phase 2)
  static void DrawCube(const Mat4 &mvp,
                       const Vec3 &color = Vec3(1.0f, 1.0f, 1.0f));
  static void DrawCube(const Camera &camera, const Transform &transform,
                       const Vec3 &color = Vec3(1.0f, 1.0f, 1.0f));

  // 3D Wireframe rendering
  static void DrawWireCube(const Mat4 &mvp,
                           const Vec3 &color = Vec3(1.0f, 1.0f, 1.0f));
  static void DrawWireCube(const Camera &camera, const Transform &transform,
                           const Vec3 &color = Vec3(1.0f, 1.0f, 1.0f));

private:
  // Phase 1 resources
  static std::shared_ptr<Shader> m_triangleShader;
  static std::shared_ptr<VertexArray> m_triangleVAO;
  static std::shared_ptr<VertexBuffer> m_triangleVBO;

  static std::shared_ptr<Shader> m_animatedShader;
  static std::shared_ptr<VertexArray> m_animatedVAO;
  static std::shared_ptr<VertexBuffer> m_animatedVBO;

  // Phase 2 3D resources
  static std::shared_ptr<Shader> m_cubeShader;
  static std::shared_ptr<VertexArray> m_cubeVAO;
  static std::shared_ptr<VertexBuffer> m_cubeVBO;
  static std::shared_ptr<IndexBuffer> m_cubeIBO;

  static std::shared_ptr<Shader> m_wireCubeShader;
  static std::shared_ptr<VertexArray> m_wireCubeVAO;
  static std::shared_ptr<VertexBuffer> m_wireCubeVBO;
  static std::shared_ptr<IndexBuffer> m_wireCubeIBO;

  // Helper methods
  static bool CreateTriangleResources();
  static bool CreateAnimatedResources();
  static bool CreateCubeResources();
  static bool CreateWireCubeResources();

  static void CleanupTriangleResources();
  static void CleanupAnimatedResources();
  static void CleanupCubeResources();
  static void CleanupWireCubeResources();
};

} // namespace Engine