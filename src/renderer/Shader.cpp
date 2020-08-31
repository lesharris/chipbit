#include "Shader.h"
#include "../core/Log.h"

#include <string>
#include <map>
#include <utility>

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

Chipbit::Shader::Shader(std::string vertexSource, std::string fragmentSource)
    : m_vertSource(std::move(vertexSource)),
      m_fragSource(std::move(fragmentSource)){
  Compile();
}

void Chipbit::Shader::Compile() {
  unsigned int vertex, fragment;

  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  auto vertSource = m_vertSource.c_str();
  auto fragSource = m_fragSource.c_str();

  glShaderSource(vertex, 1, &vertSource, nullptr);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragSource, nullptr);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Chipbit::Shader::ConfigureUniforms(const std::map<std::string, UniformTypes> &uniforms) {
  m_Uniforms.clear();

  for(auto& u : uniforms) {
    std::pair<std::string, UniformType> uniform{u.first, {u.second,0,0}};
    m_Uniforms.emplace(uniform);
  }

  use();

  for(auto& u : m_Uniforms) {
    auto location = glGetUniformLocation(ID, u.first.c_str());
    u.second.location = location;
  }
}

void Chipbit::Shader::SetUniform(const std::string &name, int value) {
  use();

  auto uniform = m_Uniforms[name];

  if(uniform.type != UniformTypes::Integer) {
    CB_WARN("ERROR::SHADER::SET_UNIFORM::INVALID_TYPE: Expected Integer");
    return;
  }

  uniform.value = value;
  updateUniform(uniform);
}

void Chipbit::Shader::SetUniform(const std::string &name, float value) {
  use();

  auto uniform = m_Uniforms[name];

  if(uniform.type != UniformTypes::Float) {
    CB_WARN("ERROR::SHADER::SET_UNIFORM::INVALID_TYPE: Expected Float");
    return;
  }

  uniform.value = value;
  updateUniform(uniform);
}

void Chipbit::Shader::SetUniform(const std::string &name, const glm::mat4 &value) {
  use();

  auto uniform = m_Uniforms[name];

  if(uniform.type != UniformTypes::Matrix4x4) {
    CB_WARN("ERROR::SHADER::SET_UNIFORM::INVALID_TYPE: Expected Matrix4x4");
    return;
  }

  uniform.value = value;
  updateUniform(uniform);
}

void Chipbit::Shader::SetUniform(const std::string &name, const glm::vec3 &value) {
  use();

  auto uniform = m_Uniforms[name];

  if(uniform.type != UniformTypes::Vec3) {
    CB_WARN("ERROR::SHADER::SET_UNIFORM::INVALID_TYPE: Expected Vec3");
    return;
  }

  uniform.value = value;
  updateUniform(uniform);
}

void Chipbit::Shader::SetUniform(const std::string &name, const glm::vec4 &value) {
  use();

  auto uniform = m_Uniforms[name];

  if(uniform.type != UniformTypes::Vec4) {
    CB_WARN("ERROR::SHADER::SET_UNIFORM::INVALID_TYPE: Expected Vec4");
    return;
  }

  uniform.value = value;
  updateUniform(uniform);
}

void Chipbit::Shader::SetUniform(const std::string &name, const glm::vec2& value) {
  use();

  auto uniform = m_Uniforms[name];

  if(uniform.type != UniformTypes::Vec2) {
    CB_WARN("ERROR::SHADER::SET_UNIFORM::INVALID_TYPE: Expected Vec2");
    return;
  }

  uniform.value = value;
  updateUniform(uniform);
}

void Chipbit::Shader::checkCompileErrors(unsigned int shader, const std::string &type) {
  int success;
  char infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
      CB_WARN("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}", type, infoLog);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
      CB_WARN("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}", type, infoLog);
    }
  }
}

void Chipbit::Shader::updateUniform(Chipbit::UniformType &uniform) {
  switch(uniform.type) {
    case UniformTypes::Integer:
      glUniform1i(uniform.location, std::get<int>(uniform.value));
      break;

    case UniformTypes::Float:
      glUniform1f(uniform.location, std::get<float>(uniform.value));
      break;

    case UniformTypes::Matrix4x4:
      glUniformMatrix4fv(uniform.location, 1, false, glm::value_ptr(std::get<glm::mat4>(uniform.value)));
      break;

    case UniformTypes::Vec2: {
      auto vec = std::get<glm::vec2>(uniform.value);
      glUniform2f(uniform.location, vec.x, vec.y);
    }
      break;

    case UniformTypes::Vec3: {
      auto vec = std::get<glm::vec3>(uniform.value);
      glUniform3f(uniform.location, vec.x, vec.y, vec.z);
    }
      break;

    case UniformTypes::Vec4: {
      auto vec = std::get<glm::vec4>(uniform.value);
      glUniform4f(uniform.location, vec.x, vec.y, vec.z, vec.w);
    }
      break;
  }
}

void Chipbit::Shader::SetVertexSource(const std::string &vertexSource) {
  m_vertSource = vertexSource;
}

void Chipbit::Shader::SetFragmentSource(const std::string &fragmentSource) {
  m_fragSource = fragmentSource;
}

void Chipbit::Shader::use() const {
  glUseProgram(ID);
}

void Chipbit::Shader::unbind() {
  glUseProgram(0);
}


