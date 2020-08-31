#pragma once

#include <glm/glm.hpp>

#ifndef _glfw3_h_
  #undef APIENTRY
  #define GLFW_INCLUDE_NONE
  #include <GLFW/glfw3.h>
#endif


namespace Chipbit {
  class Window {
  public:
    Window();
    ~Window();

    operator GLFWwindow *() { return m_Window; }

    bool ShouldClose();
    void BeginFrame();
    void EndFrame();

    [[nodiscard]] const glm::vec2& GetScreenSize() const {
      return m_ScreenSize;
    }

    [[maybe_unused]] [[nodiscard]] const glm::vec2& GetFrameSize() const {
      return m_FrameSize;
    }

    [[nodiscard]] const glm::vec2& GetScale() const {
      return m_Scale;
    }

    [[nodiscard]] const glm::vec2& GetMousePos() const {
      return m_MousePos;
    }

    [[nodiscard]] unsigned long GetFrameCount() const {
      return m_FrameCount;
    }

  private:
    void Update();
  private:
    GLFWwindow *m_Window;

    glm::vec2 m_ScreenSize{};
    glm::vec2 m_FrameSize{};
    glm::vec2 m_MousePos{};
    glm::vec2 m_Scale{};

    unsigned long m_FrameCount = 0ul;
  private:
    static void ErrorCallback(int error, const char *description);
    static void ResizeFramebufferCallback([[maybe_unused]] GLFWwindow *window, int width, int height);
  };
}