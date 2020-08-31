#include "Renderer.h"

#include <iostream>
#include <glad/glad.h>
#include "../loaders/ShaderLoader.h"
#include "../core/EventManager.h"

Chipbit::Renderer::Renderer(const Chip8& cpu) {
  m_Shader = ShaderLoader::load(
      "assets/shaders/chip8.vert.glsl",
      "assets/shaders/chip8.frag.glsl");

  GenVertices();
  m_IndexCount = GenIndices(cpu.Framebuffer());
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_Coords), m_Coords, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_IndexCount, m_Indices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  EventManager::Get().Attach<
      Events::ScreenUpdateEvent,
      &Renderer::HandleScreenUpdate
  >(this);
}

void Chipbit::Renderer::GenVertices() {
  for(auto y = 0; y < 33; y++)
    for(auto x = 0; x < 65; x++) {
      m_Coords[y * 130 + (x * 2)] = (-1.0f + x * 2.0f / 64.0f);
      m_Coords[y * 130 + (x * 2) + 1] = (0.5f - y * 2.0f / 64.0f);
    }
}

int Chipbit::Renderer::GenIndices(const std::vector<unsigned short>& framebuffer) {
  m_Indices = std::vector<unsigned int>(64 * 32 * 6, 0);

  auto index = 0;
  for(auto y = 0; y < 32; y++)
    for(auto x = 0; x < 64; x++) {
      unsigned char byte = framebuffer[y * 64 + x];

      if(byte == 1) {
        m_Indices[index++] = 65 * y + x;
        m_Indices[index++] = 65 * y + x + 1;
        m_Indices[index++] = 65 * (y + 1) + x;

        m_Indices[index++] = 65 * (y + 1) + x;
        m_Indices[index++] = 65 * (y + 1) + x + 1;
        m_Indices[index++] = 65 * y + x + 1;
      }
    }

  return index;
}

void Chipbit::Renderer::Render(const std::vector<unsigned short>& fb) {
  m_IndexCount = GenIndices(fb);
 /* if(m_UpdateIndices) {
    m_UpdateIndices = false;
    m_IndexCount = GenIndices(fb);
  }*/

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_IndexCount, m_Indices.data(), GL_DYNAMIC_DRAW);

  m_Shader->use();
  glBindVertexArray(m_VAO);

  glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void Chipbit::Renderer::HandleScreenUpdate(const Events::ScreenUpdateEvent &event) {
  m_UpdateIndices = true;
}
