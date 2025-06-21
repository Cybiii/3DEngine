#include "VertexArray.h"
#include "../Core/Logger.h"

#include <glad/glad.h>

namespace Engine {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::Float:
    return GL_FLOAT;
  case ShaderDataType::Float2:
    return GL_FLOAT;
  case ShaderDataType::Float3:
    return GL_FLOAT;
  case ShaderDataType::Float4:
    return GL_FLOAT;
  case ShaderDataType::Mat3:
    return GL_FLOAT;
  case ShaderDataType::Mat4:
    return GL_FLOAT;
  case ShaderDataType::Int:
    return GL_INT;
  case ShaderDataType::Int2:
    return GL_INT;
  case ShaderDataType::Int3:
    return GL_INT;
  case ShaderDataType::Int4:
    return GL_INT;
  case ShaderDataType::Bool:
    return GL_BOOL;
  }

  return 0;
}

class OpenGLVertexArray : public VertexArray {
public:
  OpenGLVertexArray() { glGenVertexArrays(1, &m_RendererID); }

  virtual ~OpenGLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

  virtual void Bind() const override { glBindVertexArray(m_RendererID); }

  virtual void Unbind() const override { glBindVertexArray(0); }

  virtual void
  AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override {
    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    const auto &layout = vertexBuffer->GetLayout();
    for (const auto &element : layout) {
      switch (element.Type) {
      case ShaderDataType::Float:
      case ShaderDataType::Float2:
      case ShaderDataType::Float3:
      case ShaderDataType::Float4: {
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void *)element.Offset);
        m_VertexBufferIndex++;
        break;
      }
      case ShaderDataType::Int:
      case ShaderDataType::Int2:
      case ShaderDataType::Int3:
      case ShaderDataType::Int4:
      case ShaderDataType::Bool: {
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribIPointer(m_VertexBufferIndex, element.GetComponentCount(),
                               ShaderDataTypeToOpenGLBaseType(element.Type),
                               layout.GetStride(),
                               (const void *)element.Offset);
        m_VertexBufferIndex++;
        break;
      }
      case ShaderDataType::Mat3:
      case ShaderDataType::Mat4: {
        uint8_t count = element.GetComponentCount();
        for (uint8_t i = 0; i < count; i++) {
          glEnableVertexAttribArray(m_VertexBufferIndex);
          glVertexAttribPointer(
              m_VertexBufferIndex, count,
              ShaderDataTypeToOpenGLBaseType(element.Type),
              element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
              (const void *)(element.Offset + sizeof(float) * count * i));
          glVertexAttribDivisor(m_VertexBufferIndex, 1);
          m_VertexBufferIndex++;
        }
        break;
      }
      default:
        ENGINE_LOG_ERROR("VertexArray", "Unknown ShaderDataType!");
      }
    }

    m_VertexBuffers.push_back(vertexBuffer);
  }

  virtual void
  SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override {
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
  }

  virtual const std::vector<std::shared_ptr<VertexBuffer>> &
  GetVertexBuffers() const {
    return m_VertexBuffers;
  }
  virtual const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const {
    return m_IndexBuffer;
  }

private:
  uint32_t m_RendererID;
  uint32_t m_VertexBufferIndex = 0;
  std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
  std::shared_ptr<IndexBuffer> m_IndexBuffer;
};

std::shared_ptr<VertexArray> VertexArray::Create() {
  return std::make_shared<OpenGLVertexArray>();
}

} // namespace Engine