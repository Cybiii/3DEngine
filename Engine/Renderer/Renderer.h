#pragma once

#include <memory>

namespace Engine {

class Shader;
class VertexArray;
class VertexBuffer;

class Renderer {
public:
  static bool Initialize();
  static void Shutdown();

  static void Clear();
  static void SetClearColor(float r, float g, float b, float a = 1.0f);
  static void SetViewport(int x, int y, int width, int height);

  // Phase 1: Simple triangle rendering
  static void DrawTriangle();

  // Future: Advanced rendering functions
  // static void DrawMesh(const Mesh& mesh);
  // static void Submit(const RenderCommand& command);

private:
  static std::shared_ptr<Shader> s_TriangleShader;
  static std::shared_ptr<VertexArray> s_TriangleVAO;
  static std::shared_ptr<VertexBuffer> s_TriangleVBO;

  static bool CreateTriangleResources();
  static void DestroyTriangleResources();
};

} // namespace Engine