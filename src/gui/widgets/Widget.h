#pragma once

#include "../../cpu/Chip8.h"
#include <imgui.h>
namespace Chipbit {
  class Widget {
  public:
    virtual void Render(Chip8::CPU& cpu) = 0;
  };
}
