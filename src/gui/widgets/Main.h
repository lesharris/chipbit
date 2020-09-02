#pragma once

#include "Widget.h"
#include "../../core/events/Events.h"

namespace Chipbit {
  class Main : public Widget {
  public:
    Main();
    void Render(Chip8::CPU &cpu) override;
  private:
    void HandlePausedEvent(const Events::PauseEvent& event);
  private:
    bool m_Paused = true;
  };
}