#include "Renderer.h"
#include "../Core/Camera.h"
#include "../Core/Logger.h"
#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"

#include <cmath>
#include <fstream>
#include <glad/glad.h>

namespace Engine {

// Static member definitions
std::shared_ptr<Shader> Renderer::m_triangleShader = nullptr;
std::shared_ptr<VertexArray> Renderer::m_triangleVAO = nullptr;
std::shared_ptr<VertexBuffer> Renderer::m_triangleVBO = nullptr;

std::shared_ptr<Shader> Renderer::m_animatedShader = nullptr;
std::shared_ptr<VertexArray> Renderer::m_animatedVAO = nullptr;
std::shared_ptr<VertexBuffer> Renderer::m_animatedVBO = nullptr;

// Phase 2 3D resources
std::shared_ptr<Shader> Renderer::m_cubeShader = nullptr;
std::shared_ptr<VertexArray> Renderer::m_cubeVAO = nullptr;
std::shared_ptr<VertexBuffer> Renderer::m_cubeVBO = nullptr;
std::shared_ptr<IndexBuffer> Renderer::m_cubeIBO = nullptr;

std::shared_ptr<Shader> Renderer::m_wireCubeShader = nullptr;
std::shared_ptr<VertexArray> Renderer::m_wireCubeVAO = nullptr;
std::shared_ptr<VertexBuffer> Renderer::m_wireCubeVBO = nullptr;
std::shared_ptr<IndexBuffer> Renderer::m_wireCubeIBO = nullptr;

bool Renderer::Initialize() {
  Logger::Info("Renderer", "Initializing Renderer...");

  // Initialize OpenGL state
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Set initial clear color
  Clear(0.1f, 0.1f, 0.1f, 1.0f);

  // Create triangle resources
  if (!CreateTriangleResources()) {
    Logger::Error("Renderer", "Failed to create triangle resources!");
    return false;
  }

  // Create animated shader resources for weird effects
  if (!CreateAnimatedResources()) {
    Logger::Error("Renderer", "Failed to create animated shader resources!");
    return false;
  }

  // Create 3D cube resources for Phase 2
  if (!CreateCubeResources()) {
    Logger::Error("Renderer", "Failed to create cube resources!");
    return false;
  }

  // Create wireframe cube resources
  if (!CreateWireCubeResources()) {
    Logger::Error("Renderer", "Failed to create wire cube resources!");
    return false;
  }

  Logger::Info("Renderer", "Renderer initialized successfully");
  return true;
}

void Renderer::Shutdown() {
  Logger::Info("Renderer", "Shutting down Renderer...");

  CleanupTriangleResources();
  CleanupAnimatedResources();
  CleanupCubeResources();
  CleanupWireCubeResources();

  Logger::Info("Renderer", "Renderer shutdown complete");
}

void Renderer::Clear(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
}

void Renderer::DrawTriangle() {
  if (!m_triangleShader || !m_triangleVAO) {
    Logger::Warn("Renderer", "Triangle resources not initialized!");
    return;
  }

  m_triangleShader->Bind();
  m_triangleVAO->Bind();

  glDrawArrays(GL_TRIANGLES, 0, 3);

  m_triangleVAO->Unbind();
  m_triangleShader->Unbind();
}

void Renderer::DrawAnimatedTriangle(float time) {
  if (!m_animatedShader || !m_triangleVAO) {
    Logger::Warn("Renderer", "Animated resources not initialized!");
    return;
  }

  m_animatedShader->Bind();
  m_animatedShader->SetFloat("u_Time", time);
  m_animatedShader->SetFloat("u_RotationSpeed", 2.0f);
  m_animatedShader->SetFloat("u_ColorSpeed", 3.0f);

  m_triangleVAO->Bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::DrawTriangleSpiral(float time, int count) {
  if (!m_animatedShader || !m_triangleVAO)
    return;

  m_animatedShader->Bind();
  m_triangleVAO->Bind();

  for (int i = 0; i < count; ++i) {
    float angle = (float)i / count * 6.28318f; // 2*PI
    float radius = 0.1f + (float)i / count * 0.7f;
    float spiralTime = time + (float)i * 0.1f;

    // Calculate position offset
    float x = std::cos(angle + time) * radius;
    float y = std::sin(angle + time) * radius;

    m_animatedShader->SetFloat("u_Time", spiralTime);
    m_animatedShader->SetFloat("u_OffsetX", x);
    m_animatedShader->SetFloat("u_OffsetY", y);
    m_animatedShader->SetFloat("u_Scale", 0.3f - (float)i / count * 0.2f);
    m_animatedShader->SetFloat("u_ColorPhase", (float)i / count * 6.28318f);

    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}

void Renderer::DrawColorCyclingTriangles(float time) {
  if (!m_animatedShader || !m_triangleVAO)
    return;

  m_animatedShader->Bind();
  m_triangleVAO->Bind();

  // Draw multiple triangles in a grid pattern with color cycling
  for (int x = -2; x <= 2; ++x) {
    for (int y = -2; y <= 2; ++y) {
      float offsetX = x * 0.4f;
      float offsetY = y * 0.4f;
      float distance = std::sqrt(offsetX * offsetX + offsetY * offsetY);
      float colorPhase = distance + time * 2.0f;

      m_animatedShader->SetFloat("u_Time", time);
      m_animatedShader->SetFloat("u_OffsetX", offsetX);
      m_animatedShader->SetFloat("u_OffsetY", offsetY);
      m_animatedShader->SetFloat("u_Scale", 0.15f);
      m_animatedShader->SetFloat("u_ColorPhase", colorPhase);
      m_animatedShader->SetFloat("u_RotationSpeed", 1.0f + distance);

      glDrawArrays(GL_TRIANGLES, 0, 3);
    }
  }
}

void Renderer::DrawMorphingShape(float time) {
  if (!m_animatedShader || !m_triangleVAO)
    return;

  m_animatedShader->Bind();
  m_triangleVAO->Bind();

  // Create a morphing flower-like pattern
  int petals = 8;
  for (int i = 0; i < petals; ++i) {
    float angle = (float)i / petals * 6.28318f;
    float morphTime = time + (float)i * 0.2f;
    float radius = 0.5f + std::sin(morphTime * 2.0f) * 0.3f;

    float x = std::cos(angle) * radius;
    float y = std::sin(angle) * radius;

    m_animatedShader->SetFloat("u_Time", morphTime);
    m_animatedShader->SetFloat("u_OffsetX", x);
    m_animatedShader->SetFloat("u_OffsetY", y);
    m_animatedShader->SetFloat("u_Scale", 0.2f + std::sin(morphTime) * 0.1f);
    m_animatedShader->SetFloat("u_ColorPhase", angle + time);
    m_animatedShader->SetFloat("u_RotationSpeed", 0.5f);

    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}

// Phase 2: 3D Cube rendering methods
void Renderer::DrawCube(const Mat4 &mvp, const Vec3 &color) {
  if (!m_cubeShader || !m_cubeVAO) {
    Logger::Warn("Renderer", "Cube resources not initialized!");
    return;
  }

  m_cubeShader->Bind();
  m_cubeShader->SetMat4("u_MVP", mvp);
  m_cubeShader->SetVec3("u_Color", color);

  m_cubeVAO->Bind();
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  m_cubeVAO->Unbind();
  m_cubeShader->Unbind();
}

void Renderer::DrawCube(const Camera &camera, const Transform &transform,
                        const Vec3 &color) {
  Mat4 model = transform.ToMatrix();
  Mat4 view = camera.GetViewMatrix();
  Mat4 projection = camera.GetProjectionMatrix();
  Mat4 mvp = projection * view * model;

  DrawCube(mvp, color);
}

void Renderer::DrawWireCube(const Mat4 &mvp, const Vec3 &color) {
  if (!m_wireCubeShader || !m_wireCubeVAO) {
    Logger::Warn("Renderer", "Wire cube resources not initialized!");
    return;
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  m_wireCubeShader->Bind();
  m_wireCubeShader->SetMat4("u_MVP", mvp);
  m_wireCubeShader->SetVec3("u_Color", color);

  m_wireCubeVAO->Bind();
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  m_wireCubeVAO->Unbind();
  m_wireCubeShader->Unbind();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::DrawWireCube(const Camera &camera, const Transform &transform,
                            const Vec3 &color) {
  Mat4 model = transform.ToMatrix();
  Mat4 view = camera.GetViewMatrix();
  Mat4 projection = camera.GetProjectionMatrix();
  Mat4 mvp = projection * view * model;

  DrawWireCube(mvp, color);
}

bool Renderer::CreateTriangleResources() {
  Logger::Info("Renderer", "Creating triangle resources...");

  // Triangle vertices (position + color)
  float vertices[] = {
      // Position       // Color
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom left - Red
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom right - Green
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // Top - Blue
  };

  // Create Vertex Array Object
  m_triangleVAO = VertexArray::Create();

  // Create Vertex Buffer Object
  m_triangleVBO = VertexBuffer::Create(vertices, sizeof(vertices));
  m_triangleVBO->SetLayout({{ShaderDataType::Float3, "a_Position"},
                            {ShaderDataType::Float3, "a_Color"}});

  m_triangleVAO->AddVertexBuffer(m_triangleVBO);

  // Create basic shader
  std::string vertexShaderSource = R"(
      #version 330 core
      layout (location = 0) in vec3 a_Position;
      layout (location = 1) in vec3 a_Color;
      
      out vec3 v_Color;
      
      void main() {
          gl_Position = vec4(a_Position, 1.0);
          v_Color = a_Color;
      }
  )";

  std::string fragmentShaderSource = R"(
      #version 330 core
      in vec3 v_Color;
      out vec4 FragColor;
      
      void main() {
          FragColor = vec4(v_Color, 1.0);
      }
  )";

  m_triangleShader =
      Shader::Create("BasicTriangle", vertexShaderSource, fragmentShaderSource);

  Logger::Info("Renderer", "Triangle resources created successfully");
  return true;
}

bool Renderer::CreateAnimatedResources() {
  Logger::Info("Renderer", "Creating animated shader resources...");

  // Animated vertex shader with transformations
  std::string animatedVertexSource = R"(
      #version 330 core
      layout (location = 0) in vec3 a_Position;
      layout (location = 1) in vec3 a_Color;
      
      uniform float u_Time;
      uniform float u_OffsetX;
      uniform float u_OffsetY;
      uniform float u_Scale;
      uniform float u_RotationSpeed;
      
      out vec3 v_Color;
      out float v_Time;
      
      void main() {
          float angle = u_Time * u_RotationSpeed;
          float cosA = cos(angle);
          float sinA = sin(angle);
          
          vec3 rotatedPos = vec3(
              a_Position.x * cosA - a_Position.y * sinA,
              a_Position.x * sinA + a_Position.y * cosA,
              a_Position.z
          );
          
          vec3 scaledPos = rotatedPos * u_Scale;
          vec3 finalPos = scaledPos + vec3(u_OffsetX, u_OffsetY, 0.0);
          
          gl_Position = vec4(finalPos, 1.0);
          v_Color = a_Color;
          v_Time = u_Time;
      }
  )";

  // Animated fragment shader with color cycling
  std::string animatedFragmentSource = R"(
      #version 330 core
      in vec3 v_Color;
      in float v_Time;
      
      uniform float u_ColorPhase;
      
      out vec4 FragColor;
      
      void main() {
          float r = 0.5 + 0.5 * sin(v_Time * 2.0 + u_ColorPhase);
          float g = 0.5 + 0.5 * sin(v_Time * 2.0 + u_ColorPhase + 2.094);
          float b = 0.5 + 0.5 * sin(v_Time * 2.0 + u_ColorPhase + 4.188);
          float alpha = 0.8 + 0.2 * sin(v_Time * 3.0);
          
          vec3 animatedColor = mix(v_Color, vec3(r, g, b), 0.7);
          FragColor = vec4(animatedColor, alpha);
      }
  )";

  m_animatedShader = Shader::Create("AnimatedTriangle", animatedVertexSource,
                                    animatedFragmentSource);

  Logger::Info("Renderer", "Animated shader resources created successfully");
  return true;
}

bool Renderer::CreateCubeResources() {
  Logger::Info("Renderer", "Creating cube resources...");

  // Cube vertices with positions and colors
  float cubeVertices[] = {
      // Front face
      -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 0
      0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // 1
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // 2
      -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,  // 3

      // Back face
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, // 4
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // 5
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,   // 6
      -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f   // 7
  };

  // Cube indices
  uint32_t cubeIndices[] = {// Front face
                            0, 1, 2, 2, 3, 0,
                            // Back face
                            4, 5, 6, 6, 7, 4,
                            // Left face
                            7, 3, 0, 0, 4, 7,
                            // Right face
                            1, 5, 6, 6, 2, 1,
                            // Top face
                            3, 2, 6, 6, 7, 3,
                            // Bottom face
                            0, 1, 5, 5, 4, 0};

  // Create VAO
  m_cubeVAO = VertexArray::Create();

  // Create VBO
  m_cubeVBO = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
  m_cubeVBO->SetLayout({{ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float3, "a_Color"}});

  // Create IBO
  m_cubeIBO = IndexBuffer::Create(cubeIndices, 36);

  // Add buffers to VAO
  m_cubeVAO->AddVertexBuffer(m_cubeVBO);
  m_cubeVAO->SetIndexBuffer(m_cubeIBO);

  // Load cube shader from files
  try {
    // Read shader files
    std::ifstream vertFile("../Shaders/Cube.vert");
    std::ifstream fragFile("../Shaders/Cube.frag");

    if (!vertFile.is_open() || !fragFile.is_open()) {
      Logger::Error("Renderer", "Failed to open cube shader files");
      return false;
    }

    std::string vertexSource((std::istreambuf_iterator<char>(vertFile)),
                             std::istreambuf_iterator<char>());
    std::string fragmentSource((std::istreambuf_iterator<char>(fragFile)),
                               std::istreambuf_iterator<char>());

    m_cubeShader = Shader::Create("CubeShader", vertexSource, fragmentSource);
  } catch (const std::exception &e) {
    Logger::Error("Renderer",
                  "Failed to load cube shader: " + std::string(e.what()));
    return false;
  }

  Logger::Info("Renderer", "Cube resources created successfully");
  return true;
}

bool Renderer::CreateWireCubeResources() {
  Logger::Info("Renderer", "Creating wire cube resources...");

  // Reuse the same vertex data as solid cube
  float cubeVertices[] = {
      // Front face
      -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, // 0
      0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,  // 1
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,   // 2
      -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,  // 3

      // Back face
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, // 4
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  // 5
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,   // 6
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f   // 7
  };

  // Same indices as solid cube
  uint32_t cubeIndices[] = {// Front face
                            0, 1, 2, 2, 3, 0,
                            // Back face
                            4, 5, 6, 6, 7, 4,
                            // Left face
                            7, 3, 0, 0, 4, 7,
                            // Right face
                            1, 5, 6, 6, 2, 1,
                            // Top face
                            3, 2, 6, 6, 7, 3,
                            // Bottom face
                            0, 1, 5, 5, 4, 0};

  // Create VAO
  m_wireCubeVAO = VertexArray::Create();

  // Create VBO
  m_wireCubeVBO = VertexBuffer::Create(cubeVertices, sizeof(cubeVertices));
  m_wireCubeVBO->SetLayout({{ShaderDataType::Float3, "a_Position"},
                            {ShaderDataType::Float3, "a_Color"}});

  // Create IBO
  m_wireCubeIBO = IndexBuffer::Create(cubeIndices, 36);

  // Add buffers to VAO
  m_wireCubeVAO->AddVertexBuffer(m_wireCubeVBO);
  m_wireCubeVAO->SetIndexBuffer(m_wireCubeIBO);

  // Reuse the same shader as solid cube
  try {
    // Read shader files
    std::ifstream vertFile("../Shaders/Cube.vert");
    std::ifstream fragFile("../Shaders/Cube.frag");

    if (!vertFile.is_open() || !fragFile.is_open()) {
      Logger::Error("Renderer", "Failed to open wire cube shader files");
      return false;
    }

    std::string vertexSource((std::istreambuf_iterator<char>(vertFile)),
                             std::istreambuf_iterator<char>());
    std::string fragmentSource((std::istreambuf_iterator<char>(fragFile)),
                               std::istreambuf_iterator<char>());

    m_wireCubeShader =
        Shader::Create("WireCubeShader", vertexSource, fragmentSource);
  } catch (const std::exception &e) {
    Logger::Error("Renderer",
                  "Failed to load wire cube shader: " + std::string(e.what()));
    return false;
  }

  Logger::Info("Renderer", "Wire cube resources created successfully");
  return true;
}

void Renderer::CleanupTriangleResources() {
  m_triangleShader.reset();
  m_triangleVAO.reset();
  m_triangleVBO.reset();
}

void Renderer::CleanupAnimatedResources() {
  m_animatedShader.reset();
  m_animatedVAO.reset();
  m_animatedVBO.reset();
}

void Renderer::CleanupCubeResources() {
  m_cubeShader.reset();
  m_cubeVAO.reset();
  m_cubeVBO.reset();
  m_cubeIBO.reset();
}

void Renderer::CleanupWireCubeResources() {
  m_wireCubeShader.reset();
  m_wireCubeVAO.reset();
  m_wireCubeVBO.reset();
  m_wireCubeIBO.reset();
}

} // namespace Engine