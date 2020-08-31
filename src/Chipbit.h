#pragma once

#include <memory>
#include "cpu/Chip8.h"
#include "core/Input.h"
#include "core/Window.h"
#include "renderer/Renderer.h"

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
  private:
    Chipbit() = default;
    ~Chipbit() = default;
    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Chip8> m_CPU;
    std::shared_ptr<Renderer> m_Renderer;
    std::shared_ptr<Input> m_Input;
    double m_LastFrameTime = 0.0;
    double m_DeltaTime = 0.0;
  };
}
