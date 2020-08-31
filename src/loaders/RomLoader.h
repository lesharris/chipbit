#pragma once

#include <string>
#include <memory>
#include <vector>

namespace Chipbit {
  class RomLoader {
  public:
    static std::vector<unsigned char> load(const std::string& romPath);
  };
}