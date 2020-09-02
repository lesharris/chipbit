#include "RegisterView.h"



void Chipbit::RegisterView::Render(Chip8::CPU &cpu) {
  ImGui::Begin("Registers");



  auto io = ImGui::GetIO();

  ImGui::Text("\nInternal Registers\n\n");
  ImGui::PushFont(io.Fonts->Fonts[1]);
  ImGui::Columns(3, "regs");
  ImGui::Separator();
  ImGui::Text("PC"); ImGui::NextColumn();
  ImGui::Text("I"); ImGui::NextColumn();
  ImGui::Text("SP"); ImGui::NextColumn();
  ImGui::Separator();
  ImGui::Text("0x%04X", cpu.PC); ImGui::NextColumn();
  ImGui::Text("0x%04X", cpu.I); ImGui::NextColumn();
  ImGui::Text("0x%02X", cpu.SP); ImGui::NextColumn();
  ImGui::Separator();
  ImGui::Columns(1);
  ImGui::Separator();
  ImGui::PopFont();

  ImGui::Text("\nV Registers\n\n");
  ImGui::PushFont(io.Fonts->Fonts[1]);
  ImGui::Columns(8, "vregs");
  ImGui::Separator();
  ImGui::Text("V0"); ImGui::NextColumn();
  ImGui::Text("V1"); ImGui::NextColumn();
  ImGui::Text("V2"); ImGui::NextColumn();
  ImGui::Text("V3"); ImGui::NextColumn();
  ImGui::Text("V4"); ImGui::NextColumn();
  ImGui::Text("V5"); ImGui::NextColumn();
  ImGui::Text("V6"); ImGui::NextColumn();
  ImGui::Text("V7"); ImGui::NextColumn();
  ImGui::Separator();

  for(auto i = 0; i < 8; i++) {
    ImGui::Text("0x%02X", cpu.registers[i]); ImGui::NextColumn();
  }

  ImGui::Text("V8"); ImGui::NextColumn();
  ImGui::Text("V9"); ImGui::NextColumn();
  ImGui::Text("VA"); ImGui::NextColumn();
  ImGui::Text("VB"); ImGui::NextColumn();
  ImGui::Text("VC"); ImGui::NextColumn();
  ImGui::Text("VD"); ImGui::NextColumn();
  ImGui::Text("VE"); ImGui::NextColumn();
  ImGui::Text("VF"); ImGui::NextColumn();
  ImGui::Separator();

  for(auto i = 8; i < 16; i++) {
    ImGui::Text("0x%02X", cpu.registers[i]); ImGui::NextColumn();
  }

  ImGui::PopFont();
  ImGui::Columns(1);
  ImGui::Separator();


  ImGui::Text("\nTimers\n\n");
  ImGui::PushFont(io.Fonts->Fonts[1]);
  ImGui::Columns(2, "timers");
  ImGui::Separator();
  ImGui::Text("Delay"); ImGui::NextColumn();
  ImGui::Text("Sound"); ImGui::NextColumn();
  ImGui::Separator();
  ImGui::Text("0x%02X", cpu.delay_timer); ImGui::NextColumn();
  ImGui::Text("0x%02X", cpu.sound_timer); ImGui::NextColumn();
  ImGui::Separator();
  ImGui::PopFont();
  ImGui::End();
}
