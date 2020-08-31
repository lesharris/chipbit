#pragma once

#include <glm/glm.hpp>
#include <variant>

namespace Chipbit {
  using UniformValue = std::variant<
      int,
      float,
      glm::vec2,
      glm::vec3,
      glm::vec4,
      glm::mat4>;

  enum class UniformTypes {
    Integer,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Matrix4x4
  };

  struct UniformType {
    UniformTypes type;
    unsigned int location;
    UniformValue value;
  };
}