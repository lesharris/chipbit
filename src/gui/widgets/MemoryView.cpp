#include "MemoryView.h"

#include <imgui_memory_editor.h>

void Chipbit::MemoryView::Render(Chipbit::Chip8::CPU &cpu) {
  static MemoryEditor editor;
  auto io = ImGui::GetIO();
  ImGui::PushFont(io.Fonts->Fonts[1]);
  editor.DrawWindow("Memory View", cpu.ram.data(), 4096, 0);
  ImGui::PopFont();
}
