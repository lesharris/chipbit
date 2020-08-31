#include "Renderer.h"

#include <iostream>
#include <glad/glad.h>
#include "../loaders/ShaderLoader.h"

Chipbit::Renderer::Renderer(const Chip8& cpu) {
  m_Shader = ShaderLoader::load(
      "assets/shaders/chip8.vert.glsl",
      "assets/shaders/chip8.frag.glsl");

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);

  float vertices[] = {
     -1.0,  1.0, 0.0f, 0.0f,
      1.0,  1.0, 1.0f, 0.0f,
      1.0, -1.0, 1.0f, 1.0f,
     -1.0, -1.0, 0.0f, 1.0f
  };

  int indices[] = {
      0, 1, 2,
      2, 3, 0
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>((2 * sizeof(float))));

  glGenTextures(1, &m_Texture);
  glBindTexture(GL_TEXTURE_2D, m_Texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

}

void Chipbit::Renderer::Render(const std::vector<unsigned int>& fb) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 64, 32, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, fb.data());

  m_Shader->use();

  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
