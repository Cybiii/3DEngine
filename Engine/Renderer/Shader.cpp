#include "Shader.h"
#include "../Core/Logger.h"

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>


namespace Engine {

static GLenum ShaderTypeFromString(const std::string &type) {
  if (type == "vertex")
    return GL_VERTEX_SHADER;
  if (type == "fragment" || type == "pixel")
    return GL_FRAGMENT_SHADER;

  return 0;
}

Shader::Shader(const std::string &name) : m_RendererID(0), m_Name(name) {}

Shader::Shader(const std::string &name, const std::string &vertexSrc,
               const std::string &fragmentSrc)
    : m_RendererID(0), m_Name(name) {

  std::unordered_map<uint32_t, std::string> sources;
  sources[GL_VERTEX_SHADER] = vertexSrc;
  sources[GL_FRAGMENT_SHADER] = fragmentSrc;
  Compile(sources);
}

Shader::~Shader() {
  if (m_RendererID != 0) {
    glDeleteProgram(m_RendererID);
  }
}

void Shader::Bind() const { glUseProgram(m_RendererID); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetInt(const std::string &name, int value) {
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string &name, float value) {
  glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetFloat3(const std::string &name, float x, float y, float z) {
  glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::SetFloat4(const std::string &name, float x, float y, float z,
                       float w) {
  glUniform4f(GetUniformLocation(name), x, y, z, w);
}

int Shader::GetUniformLocation(const std::string &name) const {
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  int location = glGetUniformLocation(m_RendererID, name.c_str());
  if (location == -1)
    ENGINE_LOG_WARN("Shader", "Warning: uniform '" + name + "' doesn't exist!");

  m_UniformLocationCache[name] = location;
  return location;
}

void Shader::Compile(
    const std::unordered_map<uint32_t, std::string> &shaderSources) {
  GLuint program = glCreateProgram();
  std::vector<GLuint> glShaderIDs;
  glShaderIDs.reserve(shaderSources.size());

  for (auto &kv : shaderSources) {
    GLenum type = kv.first;
    const std::string &source = kv.second;

    GLuint shader = glCreateShader(type);

    const GLchar *sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

      glDeleteShader(shader);

      ENGINE_LOG_ERROR("Shader", "Shader compilation failure!");
      ENGINE_LOG_ERROR("Shader", infoLog.data());
      break;
    }

    glAttachShader(program, shader);
    glShaderIDs.push_back(shader);
  }

  m_RendererID = program;

  // Link our program
  glLinkProgram(program);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(program);

    for (auto id : glShaderIDs)
      glDeleteShader(id);

    ENGINE_LOG_ERROR("Shader", "Shader link failure!");
    ENGINE_LOG_ERROR("Shader", infoLog.data());
    return;
  }

  for (auto id : glShaderIDs) {
    glDetachShader(program, id);
    glDeleteShader(id);
  }

  ENGINE_LOG_INFO("Shader", "Shader '" + m_Name + "' compiled successfully");
}

std::shared_ptr<Shader> Shader::Create(const std::string &name,
                                       const std::string &vertexSrc,
                                       const std::string &fragmentSrc) {
  return std::make_shared<Shader>(name, vertexSrc, fragmentSrc);
}

} // namespace Engine