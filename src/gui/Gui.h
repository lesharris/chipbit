#pragma once

#include "../cpu/Chip8.h"

namespace Chipbit {
  class Gui {
  public:
    Gui();

    void Render(Chip8::CPU& cpu);
    void BeginFrame();
    void EndFrame();
    void Destroy();
  private:
    static void SetImGuiStyle();
  };
}
