#include "Renderer.h"
#include "../Core/Logger.h"
#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glad/glad.h>
#include <memory>

namespace Engine {

// Static member definitions
std::shared_ptr<Shader> Renderer::s_TriangleShader;
std::shared_ptr<VertexArray> Renderer::s_TriangleVAO;
std::shared_ptr<VertexBuffer> Renderer::s_TriangleVBO;

bool Renderer::Initialize() {
  ENGINE_LOG_INFO("Renderer", "Initializing Renderer...");

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);

  // Set initial clear color
  SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  // Create triangle resources
  if (!CreateTriangleResources()) {
    ENGINE_LOG_ERROR("Renderer", "Failed to create triangle resources!");
    return false;
  }

  ENGINE_LOG_INFO("Renderer", "Renderer initialized successfully");
  return true;
}

void Renderer::Shutdown() {
  ENGINE_LOG_INFO("Renderer", "Shutting down Renderer...");

  DestroyTriangleResources();

  ENGINE_LOG_INFO("Renderer", "Renderer shutdown complete");
}

void Renderer::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void Renderer::SetViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

void Renderer::DrawTriangle() {
  if (!s_TriangleShader || !s_TriangleVAO) {
    ENGINE_LOG_ERROR("Renderer", "Triangle resources not initialized!");
    return;
  }

  s_TriangleShader->Bind();
  s_TriangleVAO->Bind();

  glDrawArrays(GL_TRIANGLES, 0, 3);

  s_TriangleVAO->Unbind();
  s_TriangleShader->Unbind();
}

bool Renderer::CreateTriangleResources() {
  ENGINE_LOG_INFO("Renderer", "Creating triangle resources...");

  // Triangle vertices (position + color)
  float vertices[] = {
      // positions        // colors
      0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // top (red)
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left (green)
      0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // bottom right (blue)
  };

  // Create and bind vertex array
  s_TriangleVAO = VertexArray::Create();
  s_TriangleVAO->Bind();

  // Create vertex buffer
  s_TriangleVBO = VertexBuffer::Create(vertices, sizeof(vertices));
  BufferLayout layout = {{ShaderDataType::Float3, "a_Position"},
                         {ShaderDataType::Float3, "a_Color"}};
  s_TriangleVBO->SetLayout(layout);
  s_TriangleVAO->AddVertexBuffer(s_TriangleVBO);

  s_TriangleVAO->Unbind();

  // Create shader
  std::string vertexSrc = R"(
            #version 450 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec3 a_Color;
            
            out vec3 v_Color;
            
            void main()
            {
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

  std::string fragmentSrc = R"(
            #version 450 core
            
            layout(location = 0) out vec4 color;
            
            in vec3 v_Color;
            
            void main()
            {
                color = vec4(v_Color, 1.0);
            }
        )";

  s_TriangleShader =
      std::make_shared<Shader>("BasicTriangle", vertexSrc, fragmentSrc);

  ENGINE_LOG_INFO("Renderer", "Triangle resources created successfully");
  return true;
}

void Renderer::DestroyTriangleResources() {
  s_TriangleShader.reset();
  s_TriangleVAO.reset();
  s_TriangleVBO.reset();
}

} // namespace Engine