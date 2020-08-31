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

    void Render(const std::vector<unsigned short>& fb);
  private:
    void GenVertices();
    int GenIndices(const std::vector<unsigned short>& framebuffer);
    void HandleScreenUpdate(const Events::ScreenUpdateEvent& event);
  private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;

    std::shared_ptr<Shader> m_Shader;

    float m_Coords[4290]{};

    int m_IndexCount = 0;
    std::vector<unsigned int> m_Indices{};

    bool m_UpdateIndices = false;
  };
}