#include "Main.h"

#include "ImGuiFileDialog.h"

#include "../../core/EventManager.h"

Chipbit::Main::Main() {
  EventManager::Get().Attach<
      Events::PauseEvent,
      &Main::HandlePausedEvent
  >(this);
}

void Chipbit::Main::Render(Chipbit::Chip8::CPU &cpu) {
  if(m_Paused && ImGui::BeginMainMenuBar()) {

    if (ImGui::BeginMenu("File")) {

      if (ImGui::MenuItem("Open ROM...")) {
        igfd::ImGuiFileDialog::Instance()->OpenDialog("OpenRom", "Choose ROM...", ".*", ".");
      }

      ImGui::Separator();

      if(ImGui::MenuItem("Reset")) {
        EventManager::Dispatcher().trigger<Events::ResetEvent>();
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Quit")) {
        EventManager::Dispatcher().trigger<Events::QuitEvent>();
      }

      ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Tools")) {
      if(ImGui::MenuItem("View Registers")) {}
      if(ImGui::MenuItem("View Stack")) {}
      if(ImGui::MenuItem("View Memory")) {}

      ImGui::EndMenu();
    }

    if(ImGui::BeginMenu("Speed")) {
      static unsigned int selected = 20;
      if(ImGui::MenuItem("7 ticks/frame", nullptr, selected == 7)) {
        selected = 7;
        EventManager::Dispatcher().trigger<Events::UpdateTPFEvent>(7u);
      }
      if(ImGui::MenuItem("15 ticks/frame", nullptr, selected == 15)) {
        selected = 15;
        EventManager::Dispatcher().trigger<Events::UpdateTPFEvent>(15u);
      }
      if(ImGui::MenuItem("20 ticks/frame", nullptr, selected == 20)) {
        selected = 20;
        EventManager::Dispatcher().trigger<Events::UpdateTPFEvent>(20u);
      }
      if(ImGui::MenuItem("30 ticks/frame", nullptr, selected == 30)) {
        selected = 30;
        EventManager::Dispatcher().trigger<Events::UpdateTPFEvent>(30u);
      }
      if(ImGui::MenuItem("100 ticks/frame", nullptr, selected == 100)) {
        selected = 100;
        EventManager::Dispatcher().trigger<Events::UpdateTPFEvent>(100u);
      }
      if(ImGui::MenuItem("200 ticks/frame", nullptr, selected == 200)) {
        selected = 200;
        EventManager::Dispatcher().trigger<Events::UpdateTPFEvent>(200u);
      }
      if(ImGui::MenuItem("500 ticks/frame", nullptr, selected == 500)) {
        selected = 500;
        EventManager::Dispatcher().trigger<Events::UpdateTPFEvent>(500u);
      }
      if(ImGui::MenuItem("1000 ticks/frame", nullptr, selected == 1000)) {
        selected = 1000;
        EventManager::Dispatcher().trigger<Events::UpdateTPFEvent>(1000u);
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  if (igfd::ImGuiFileDialog::Instance()->FileDialog("OpenRom")) {
    if (igfd::ImGuiFileDialog::Instance()->IsOk) {
      std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
      EventManager::Dispatcher().trigger<Events::LoadRomEvent>(filePathName);
    }


    igfd::ImGuiFileDialog::Instance()->CloseDialog("OpenRom");
  }
}

void Chipbit::Main::HandlePausedEvent(const Events::PauseEvent& event) {
  m_Paused = event.paused;
}
