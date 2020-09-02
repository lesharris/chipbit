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
        igfd::ImGuiFileDialog::Instance()->OpenDialog("OpenRom", "Choose ROM...", "", ".");
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
