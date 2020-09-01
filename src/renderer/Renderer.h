#pragma once

#include <vector>
#include <memory>
#include "Shader.h"
#include "../cpu/Chip8.h"
#include "../core/events/Events.h"

namespace Chipbit {
  class Renderer {
  public:
    explicit Renderer(const Chip8& cpu);

    void Draw(const std::vector<unsigned int>& fb);
    void Render();
  private:
    void HandleScreenResize(const Events::WindowResizedEvent& event) const;
  private:
    unsigned int m_FBO = 0;
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_ScreenTexture = 0;

    unsigned int m_C8VAO = 0;
    unsigned int m_C8VBO = 0;
    unsigned int m_C8EBO = 0;
    unsigned int m_C8Texture = 0;

    std::shared_ptr<Shader> m_C8Shader;
    std::shared_ptr<Shader> m_ScreenShader;
  };
}