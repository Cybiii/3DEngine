#pragma once

#include "Buffer.h"
#include "Math/Math.h"
#include "VertexArray.h"
#include <memory>
#include <string>
#include <vector>


namespace Engine {

// Vertex structure for 3D meshes
struct Vertex {
  Vec3 Position;
  Vec3 Normal;
  Vec2 TexCoords;
  Vec3 Color;

  Vertex()
      : Position(0.0f), Normal(0.0f, 1.0f, 0.0f), TexCoords(0.0f), Color(1.0f) {
  }

  Vertex(const Vec3 &pos, const Vec3 &normal = Vec3(0.0f, 1.0f, 0.0f),
         const Vec2 &texCoords = Vec2(0.0f), const Vec3 &color = Vec3(1.0f))
      : Position(pos), Normal(normal), TexCoords(texCoords), Color(color) {}
};

class Mesh {
public:
  // Mesh data
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  // Constructor
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<uint32_t> &indices);
  ~Mesh();

  // Disable copy constructor and assignment
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  // Enable move constructor and assignment
  Mesh(Mesh &&other) noexcept;
  Mesh &operator=(Mesh &&other) noexcept;

  // Rendering
  void Bind() const;
  void Unbind() const;
  void Draw() const;

  // Getters
  uint32_t GetVertexCount() const {
    return static_cast<uint32_t>(vertices.size());
  }
  uint32_t GetIndexCount() const {
    return static_cast<uint32_t>(indices.size());
  }

  // Static factory methods for common shapes
  static std::unique_ptr<Mesh> CreateCube(float size = 1.0f);
  static std::unique_ptr<Mesh> CreateSphere(float radius = 1.0f,
                                            uint32_t segments = 32);
  static std::unique_ptr<Mesh> CreatePlane(float width = 1.0f,
                                           float height = 1.0f);
  static std::unique_ptr<Mesh> CreateQuad();

private:
  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VBO;
  std::unique_ptr<IndexBuffer> m_IBO;

  void SetupMesh();
};

} // namespace Engine