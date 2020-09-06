#include "Renderer.h"

#include <glad/glad.h>
#include "../loaders/ShaderLoader.h"
#include "../Chipbit.h"
#include "../core/Log.h"
#include "../core/EventManager.h"


Chipbit::Renderer::Renderer(const Chip8& cpu) {
  m_C8Shader = ShaderLoader::load(
      "assets/shaders/chip8.vert.glsl",
      "assets/shaders/chip8.frag.glsl");

  m_C8Shader->ConfigureUniforms({
    {"bgColor", UniformTypes::Vec3},
    {"l1Color", UniformTypes::Vec3},
    {"l2Color", UniformTypes::Vec3},
    {"combColor", UniformTypes::Vec3},

    {"layer1", UniformTypes::Integer},
    {"layer2", UniformTypes::Integer}
  });

  m_C8Shader->use();
  m_C8Shader->SetUniform("bgColor", m_BGColor);
  m_C8Shader->SetUniform("l1Color", m_L1Color);
  m_C8Shader->SetUniform("l2Color", m_L2Color);
  m_C8Shader->SetUniform("combColor", m_CombColor);
  m_C8Shader->SetUniform("layer1", 0);
  m_C8Shader->SetUniform("layer2", 1);
  Shader::unbind();

  m_ScreenShader = ShaderLoader::load(
      "assets/shaders/chip8-screen.vert.glsl",
      "assets/shaders/chip8-screen.frag.glsl");

  m_ScreenShader->ConfigureUniforms({
   {"screenTexture", UniformTypes::Integer}
  });

  m_ScreenShader->use();
  m_ScreenShader->SetUniform("screenTexture", 0);
  Shader::unbind();

  glGenVertexArrays(1, &m_C8VAO);
  glGenBuffers(1, &m_C8VBO);
  glGenBuffers(1, &m_C8EBO);

  glBindVertexArray(m_C8VAO);

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

  glBindBuffer(GL_ARRAY_BUFFER, m_C8VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_C8EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>((2 * sizeof(float))));

  glGenTextures(1, &m_L1Texture);
  glBindTexture(GL_TEXTURE_2D, m_L1Texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glGenTextures(1, &m_L2Texture);
  glBindTexture(GL_TEXTURE_2D, m_L2Texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  float screenVertices[] = {
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
       1.0f, -1.0f,  1.0f, 0.0f,

      -1.0f,  1.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  1.0f, 1.0f
  };

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  glGenFramebuffers(1, &m_FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

  auto screenSize = Chipbit::Get().GetWindow().GetScreenSize();
  auto screenScale = Chipbit::Get().GetWindow().GetScale();

  glGenTextures(1, &m_ScreenTexture);
  glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenSize.x * screenScale.x, screenSize.y * screenScale.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ScreenTexture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    CB_ERROR("Could not generate framebuffer!");
  }

  glClearColor(m_BGColor.r, m_BGColor.g, m_BGColor.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  EventManager::Get().Attach<
      Events::WindowResizedEvent,
      &Renderer::HandleScreenResize
  >(this);
}

void Chipbit::Renderer::Draw(const std::vector<std::vector<unsigned char>>& layers) {
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

  glClearColor(m_BGColor.r, m_BGColor.g, m_BGColor.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_L1Texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 128, 64, 0, GL_RED, GL_UNSIGNED_BYTE, layers[0].data());

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_L2Texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 128, 64, 0, GL_RED, GL_UNSIGNED_BYTE, layers[1].data());

 // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 128, 64, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, fb.data());

  m_C8Shader->use();
  m_C8Shader->SetUniform("bgColor", m_BGColor);
  m_C8Shader->SetUniform("l1Color", m_L1Color);
  m_C8Shader->SetUniform("l2Color", m_L2Color);
  m_C8Shader->SetUniform("combColor", m_CombColor);

  glBindVertexArray(m_C8VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void Chipbit::Renderer::Render() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  m_ScreenShader->use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void Chipbit::Renderer::HandleScreenResize(const Events::WindowResizedEvent &event) const {
  auto& cpu = Chipbit::Get().GetCPU();
  auto& window = Chipbit::Get().GetWindow();
  auto screenScale = window.GetScale();

  glBindTexture(GL_TEXTURE_2D, m_ScreenTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
               (float)event.width * screenScale.x, (float)event.height * screenScale.y,
               0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

  glClearColor(m_BGColor.r, m_BGColor.g, m_BGColor.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
