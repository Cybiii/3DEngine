#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine {

class Shader {
public:
  Shader(const std::string &name);
  Shader(const std::string &name, const std::string &vertexSrc,
         const std::string &fragmentSrc);
  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  // Uniform setters
  void SetInt(const std::string &name, int value);
  void SetFloat(const std::string &name, float value);
  void SetFloat3(const std::string &name, float x, float y, float z);
  void SetFloat4(const std::string &name, float x, float y, float z, float w);

  // Phase 2: 3D uniform setters
  void SetMat4(const std::string &name, const Mat4 &matrix);
  void SetVec3(const std::string &name, const Vec3 &vector);

  const std::string &GetName() const { return m_Name; }

  static std::shared_ptr<Shader> Create(const std::string &name,
                                        const std::string &vertexSrc,
                                        const std::string &fragmentSrc);

private:
  uint32_t m_RendererID;
  std::string m_Name;
  mutable std::unordered_map<std::string, int> m_UniformLocationCache;

  std::string ReadFile(const std::string &filepath);
  std::unordered_map<uint32_t, std::string>
  PreProcess(const std::string &source);
  void Compile(const std::unordered_map<uint32_t, std::string> &shaderSources);

  int GetUniformLocation(const std::string &name) const;
};

} // namespace Engine