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

    void Render(const std::vector<unsigned int>& fb);
  private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;
    unsigned int m_Texture = 0;

    std::shared_ptr<Shader> m_Shader;
  };
}