#pragma once

#include "Widget.h"

namespace Chipbit {
  class MemoryView : public Widget {
  public:
    void Render(Chip8::CPU &cpu) override;
  };
}
