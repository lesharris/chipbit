#include "Main.h"

#include "ImGuiFileDialog.h"
#include "../../core/Log.h"

void Chipbit::Main::Render(Chipbit::Chip8::CPU &cpu) {
  if(ImGui::BeginMainMenuBar()) {

    if (ImGui::BeginMenu("File")) {

      if (ImGui::MenuItem("Open ROM...")) {
        igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", "", ".");
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Quit")) {
        CB_INFO("Quit");
      }

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  // display
  if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey")) {
    // action if OK
    if (igfd::ImGuiFileDialog::Instance()->IsOk) {
      std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();
      // action

      CB_INFO("PathName: {0}, Path: {1}", filePathName, filePath);
    }
    // close
    igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
  }
}
