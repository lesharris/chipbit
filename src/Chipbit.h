#pragma once

#include <memory>
#include "cpu/Chip8.h"
#include "core/Input.h"
#include "core/Window.h"
#include "renderer/Renderer.h"
#include "gui/Gui.h"

#include "core/events/Events.h"
#include <glm/glm.hpp>

namespace Chipbit {
  class Chipbit {
  public:
    static Chipbit& Get() {
      static Chipbit instance;

      return instance;
    }

    void Initialize();
    void Run();

    Window& GetWindow() { return *m_Window; }
    Chip8::CPU& GetCPU() { return m_CPU->Get(); }
  private:
    Chipbit() = default;
    ~Chipbit() = default;

    void HandleQuitEvent(const Events::QuitEvent& event);
    void HandlePauseEvent(const Events::PauseEvent& event);
    void HandleLoadRomEvent(const Events::LoadRomEvent& event);
    void HandleUpdateTPFEvent(const Events::UpdateTPFEvent &event);
    void HandleResetEvent(const Events::ResetEvent &event);
  private:
    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Chip8> m_CPU;
    std::shared_ptr<Renderer> m_Renderer;
    std::shared_ptr<Input> m_Input;
    std::shared_ptr<Gui> m_GUI;

    unsigned int m_TicksPerFrame = 20;

    double m_LastFrameTime = 0.0;
    double m_DeltaTime = 0.0;

    bool m_SingleStepMode = false;
    bool m_Paused = true;
    bool m_PauseLastFrame = false;
  };
}
