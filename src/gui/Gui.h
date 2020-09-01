#pragma once

namespace Chipbit {
  class Gui {
  public:
    Gui();

    void Render();
    void BeginFrame();
    void EndFrame();
    void Destroy();
  private:
    static void SetImGuiStyle();
  };
}
