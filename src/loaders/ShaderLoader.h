#pragma once

#include <memory>
#include <string>

#include "../renderer/Shader.h"

namespace Chipbit {
  class ShaderLoader {
  public:
    static std::shared_ptr<Shader> load(const std::string& vertexPath, const std::string& fragmentPath);
  };
}
