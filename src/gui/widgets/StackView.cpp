#include "StackView.h"

#include <fmt/format.h>

void Chipbit::StackView::Render(Chipbit::Chip8::CPU &cpu) {
  ImGui::Begin("Stack View");
  ImGui::Text("Stack");
  ImGui::Columns(2, "stack");
  ImGui::Separator();

  auto io = ImGui::GetIO();
  ImGui::PushFont(io.Fonts->Fonts[1]);
  static int selected = -1;

  selected = cpu.SP;

  for(auto i = 0; i < 16; i++) {
    ImGui::Selectable(fmt::format("{0}{1:#02X}", (i == cpu.SP) ? "*" : " ", i).c_str(), i == cpu.SP, ImGuiSelectableFlags_SpanAllColumns);
    ImGui::NextColumn();

    ImGui::Text("0x%04X", cpu.stack[i]); ImGui::NextColumn();

    ImGui::Separator();
  }

  ImGui::Columns(1);
  ImGui::Separator();
  ImGui::PopFont();
  ImGui::End();
}
