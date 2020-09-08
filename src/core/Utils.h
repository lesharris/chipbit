#pragma once

#include <vector>
#include <string>

namespace Chipbit {
  class StringUtils {
  public:
    static std::vector<std::string> SplitLines(const std::string& text);
  };
}