#include "StackView.h"

void Chipbit::StackView::Render(Chipbit::Chip8::CPU &cpu) {
  ImGui::Begin("Stack View");
  ImGui::Text("Stack");
  ImGui::Columns(2, "stack");
  ImGui::Separator();

  static int selected = -1;

  selected = cpu.sp;

  for(auto i = 0; i < 16; i++) {
    ImGui::Selectable(cpu.sp == i ? "*" : " ", i == cpu.sp, ImGuiSelectableFlags_SpanAllColumns);
    ImGui::NextColumn();

    ImGui::Text("0x%04X", cpu.stack[i]); ImGui::NextColumn();

    ImGui::Separator();
  }

  ImGui::Columns(1);
  ImGui::Separator();
  ImGui::End();
}
