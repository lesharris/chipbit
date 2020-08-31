#include "Chipbit.h"

#include "loaders/RomLoader.h"

void Chipbit::Chipbit::Initialize() {
  m_Window = std::make_shared<Window>();
  m_CPU = std::make_shared<Chip8>();
  m_Renderer = std::make_shared<Renderer>(*m_CPU);
  m_Input = std::make_shared<Input>();
}

void Chipbit::Chipbit::Run() {
  auto rom = RomLoader::load("assets/roms/INVADERS");
  m_CPU->Load(rom, 0x200);

  while(!m_Window->ShouldClose()) {
    const double time = glfwGetTime();
    m_DeltaTime += (time - m_LastFrameTime) / (1.0 / 60.0);
    m_LastFrameTime = time;

    glfwPollEvents();

    {
      std::vector<unsigned char> keyBuf(16, 0);
      auto keys = m_Input->poll();
      m_CPU->ResetKeys();

      for (const auto key : keys)
        keyBuf[ActionToKey[key]] = 1;

      m_CPU->SetKeys(keyBuf);
    }

    while(m_DeltaTime >= 1.0) {
      m_CPU->TickTimers();
      m_DeltaTime--;
    }

    m_CPU->Tick();

    if(m_CPU->Draw()) {
      m_CPU->ClearDraw();
      m_Window->BeginFrame();
      m_Renderer->Render(m_CPU->Framebuffer());
      m_Window->EndFrame();
    }
  }
}
