#include "Buffer.h"
#include "../Core/Logger.h"

#include <glad/glad.h>

namespace Engine {

/////////////////////////////////////////////////////////////////////////////
// VertexBuffer /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class OpenGLVertexBuffer : public VertexBuffer {
public:
  OpenGLVertexBuffer(uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
  }

  OpenGLVertexBuffer(float *vertices, uint32_t size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  virtual ~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

  virtual void Bind() const override {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  }

  virtual void Unbind() const override { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  virtual void SetData(const void *data, uint32_t size) override {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
  }

  virtual const BufferLayout &GetLayout() const override { return m_Layout; }
  virtual void SetLayout(const BufferLayout &layout) override {
    m_Layout = layout;
  }

private:
  uint32_t m_RendererID;
  BufferLayout m_Layout;
};

std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size) {
  return std::make_shared<OpenGLVertexBuffer>(size);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(float *vertices,
                                                   uint32_t size) {
  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
}

/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class OpenGLIndexBuffer : public IndexBuffer {
public:
  OpenGLIndexBuffer(uint32_t *indices, uint32_t count) : m_Count(count) {
    glGenBuffers(1, &m_RendererID);

    // GL_ELEMENT_ARRAY_BUFFER is not valid without a VAO bound
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
                 GL_STATIC_DRAW);
  }

  virtual ~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

  virtual void Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  }

  virtual void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

  virtual uint32_t GetCount() const { return m_Count; }

private:
  uint32_t m_RendererID;
  uint32_t m_Count;
};

std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t *indices,
                                                 uint32_t count) {
  return std::make_shared<OpenGLIndexBuffer>(indices, count);
}

} // namespace Engine