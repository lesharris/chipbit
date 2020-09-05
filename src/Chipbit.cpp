#include "Chipbit.h"

#include "loaders/RomLoader.h"
#include "core/EventManager.h"
#include "core/Log.h"

void Chipbit::Chipbit::Initialize() {
  m_Window = std::make_shared<Window>();
  m_CPU = std::make_shared<Chip8>();
  m_Renderer = std::make_shared<Renderer>(*m_CPU);
  m_Input = std::make_shared<Input>();
  m_GUI = std::make_shared<Gui>();

  EventManager::Get().Attach<
      Events::QuitEvent,
      &Chipbit::HandleQuitEvent
  >(this);

  EventManager::Get().Attach<
      Events::PauseEvent,
      &Chipbit::HandlePauseEvent
  >(this);

  EventManager::Get().Attach<
      Events::LoadRomEvent,
      &Chipbit::HandleLoadRomEvent
  >(this);
}

void Chipbit::Chipbit::Run() {
  while(!m_Window->ShouldClose()) {
    const double time = glfwGetTime();
    m_DeltaTime += (time - m_LastFrameTime) / (1.0 / 60.0);
    m_LastFrameTime = time;

    m_Window->BeginFrame();

    {
      std::vector<unsigned char> keyBuf(16, 0);
      auto keys = m_Input->poll();
      bool pauseHit = false;
      std::for_each(keys.begin(), keys.end(), [&](Action action) {
        pauseHit = false;

        switch(action) {
          case Action::Pause:
            pauseHit = true;
            if(!m_PauseLastFrame) {
              m_Paused = !m_Paused;
              EventManager::Dispatcher().trigger<Events::PauseEvent>(m_Paused);
            }
            break;

          default:
            if(!m_Paused)
              keyBuf[ActionToKey[action]] = 1;
            break;
        }
      });

      m_PauseLastFrame = pauseHit;

      if(!m_Paused)
        m_CPU->SetKeys(keyBuf);
    }

    if(!m_Paused) {
      {
        // Tick timers at 60hz
        while (m_DeltaTime >= 1.0) {
          m_CPU->TickTimers();
          m_DeltaTime--;
        }

        for (auto i = 0; i < 20; i++)
          m_CPU->Tick();

        // Update Framebuffer texture
        if (m_CPU->Draw()) {
          m_CPU->ClearDraw();
          m_Renderer->Draw(m_CPU->Framebuffer());
        }
      }
    }

    // Render Framebuffer texture every frame
    m_Renderer->Render();

    m_GUI->Render(m_CPU->Get());

    m_Window->EndFrame();
  }
}

void Chipbit::Chipbit::HandlePauseEvent(const Events::PauseEvent &event) {
  m_Paused = event.paused;
}

void Chipbit::Chipbit::HandleQuitEvent(const Events::QuitEvent& event) {
  glfwSetWindowShouldClose(*m_Window, 1);
}

void Chipbit::Chipbit::HandleLoadRomEvent(const Events::LoadRomEvent &event) {
  auto rom = RomLoader::load(event.rom);
  m_CPU->Get().Reset();
  m_CPU->Load(rom, 0x200);

  EventManager::Dispatcher().trigger<Events::PauseEvent>(false);
}

