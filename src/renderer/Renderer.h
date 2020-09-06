#pragma once

#include <vector>
#include <memory>
#include "Shader.h"
#include "../cpu/Chip8.h"
#include "../core/events/Events.h"
#include <glm/glm.hpp>

namespace Chipbit {
  class Renderer {
  public:
    explicit Renderer(const Chip8& cpu);

    void Draw(const std::vector<std::vector<unsigned char>>& layers);
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
    unsigned int m_L1Texture = 0;
    unsigned int m_L2Texture = 0;

    std::shared_ptr<Shader> m_C8Shader;
    std::shared_ptr<Shader> m_ScreenShader;

    glm::vec3 m_BGColor{202.0f / 255.0f, 220.0f / 255.0f, 159.0f / 255.0f};
    glm::vec3 m_L1Color { 48.0f/ 255.0f, 98.0f / 255.0f, 48.0f / 255.0f};
    glm::vec3 m_L2Color { 139.0f / 255.0f, 172.0f / 255.0f, 15.0f / 255.0f};
    glm::vec3 m_CombColor { 155.0f / 255.0f, 188.0f / 255.0f, 15.0f / 255.0f};
  };
}