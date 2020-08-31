#pragma once

#include <glm/glm.hpp>
#include "UniformType.h"
#include <string>
#include <map>

namespace Chipbit {
  class Shader {
  public:
    unsigned int ID = 0;

    Shader() = default;

    Shader(std::string vertexSource, std::string fragmentSource);

    void Compile();

    void ConfigureUniforms(const std::map<std::string, UniformTypes>& uniforms);

    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const glm::mat4& value);
    void SetUniform(const std::string& name, const glm::vec2& value);
    void SetUniform(const std::string& name, const glm::vec3& value);
    void SetUniform(const std::string& name, const glm::vec4& value);

    static void updateUniform(UniformType &uniform);

    void SetVertexSource(const std::string& vertexSource);

    void SetFragmentSource(const std::string& fragmentSource);

    void use() const;
    static void unbind();

  private:
    static void checkCompileErrors(unsigned int shader, const std::string &type);

  private:
    std::string m_vertSource;
    std::string m_fragSource;
    std::map<std::string, UniformType> m_Uniforms{};
  };
}
