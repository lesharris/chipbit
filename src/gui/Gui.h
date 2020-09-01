#pragma once

#include <vector>
#include <memory>

#include "../cpu/Chip8.h"
#include "widgets/Widget.h"

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
  private:
    std::vector<std::shared_ptr<Widget>> m_Widgets{};
  };
}
