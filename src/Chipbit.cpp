#include "Chipbit.h"

#include "loaders/RomLoader.h"

void Chipbit::Chipbit::Initialize() {
  m_Window = std::make_shared<Window>();
  m_CPU = std::make_shared<Chip8>();
  m_Renderer = std::make_shared<Renderer>(*m_CPU);
  m_Input = std::make_shared<Input>();
  m_GUI = std::make_shared<Gui>();
}

void Chipbit::Chipbit::Run() {
  auto rom = RomLoader::load("assets/roms/TETRIS");
  m_CPU->Load(rom, 0x200);

  while(!m_Window->ShouldClose()) {
    const double time = glfwGetTime();
    m_DeltaTime += (time - m_LastFrameTime) / (1.0 / 60.0);
    m_LastFrameTime = time;

    m_Window->BeginFrame();
    {
      {
        std::vector<unsigned char> keyBuf(16, 0);
        auto keys = m_Input->poll();

        std::for_each(keys.begin(), keys.end(), [&](Action key) {
          keyBuf[ActionToKey[key]] = 1;
        });

        m_CPU->SetKeys(keyBuf);
      }

      // Tick timers at 60hz
      while (m_DeltaTime >= 1.0) {
        m_CPU->TickTimers();
        m_DeltaTime--;
      }

      for(auto i = 0; i < 8; i++)
        m_CPU->Tick();

      // Update Framebuffer texture
      if(m_CPU->Draw()) {
        m_CPU->ClearDraw();
        m_Renderer->Draw(m_CPU->Framebuffer());
      }

      // Render Framebuffer texture every frame
      m_Renderer->Render();

      m_GUI->Render(m_CPU->Get());
    }
    m_Window->EndFrame();
  }
}

