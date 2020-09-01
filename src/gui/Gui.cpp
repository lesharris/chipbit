#include "Gui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <imgui_memory_editor.h>

#include "../Chipbit.h"

Chipbit::Gui::Gui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-Regular.ttf", 16.0f);
  io.Fonts->AddFontFromFileTTF("assets/fonts/CascadiaMono.ttf", 16.0f);

  ImGui_ImplGlfw_InitForOpenGL(Chipbit::Get().GetWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  SetImGuiStyle();
}

void Chipbit::Gui::BeginFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Chipbit::Gui::EndFrame() {
  ImGuiIO& io = ImGui::GetIO();
  auto& app = Chipbit::Get();
  io.DisplaySize = ImVec2((float) app.GetWindow().GetScreenSize().x, (float) app.GetWindow().GetScreenSize().y);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void Chipbit::Gui::Destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Chipbit::Gui::Render(Chip8::CPU& cpu) {
  static MemoryEditor editor;

  BeginFrame();

  ImGui::ShowDemoWindow();

  ImGui::Begin("Registers");

  ImGui::Text("\nInternal Registers\n\n");
  ImGui::Columns(3, "regs");
  ImGui::Separator();
  ImGui::Text("PC"); ImGui::NextColumn();
  ImGui::Text("I"); ImGui::NextColumn();
  ImGui::Text("SP"); ImGui::NextColumn();
  ImGui::Separator();
  ImGui::Text("0x%04X", cpu.PC); ImGui::NextColumn();
  ImGui::Text("0x%04X", cpu.I); ImGui::NextColumn();
  ImGui::Text("0x%02X", cpu.sp); ImGui::NextColumn();
  ImGui::Separator();
  ImGui::Columns(1);
  ImGui::Separator();

  ImGui::Text("\nV Registers\n\n");
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

  ImGui::Columns(1);
  ImGui::Separator();


  ImGui::Text("\nTimers\n\n");
  ImGui::Columns(2, "timers");
  ImGui::Separator();
  ImGui::Text("Delay"); ImGui::NextColumn();
  ImGui::Text("Sound"); ImGui::NextColumn();
  ImGui::Separator();
  ImGui::Text("0x%02X", cpu.delay_timer); ImGui::NextColumn();
  ImGui::Text("0x%02X", cpu.sound_timer); ImGui::NextColumn();
  ImGui::Separator();
  ImGui::End();

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

  auto io = ImGui::GetIO();
  ImGui::PushFont(io.Fonts->Fonts[1]);
  editor.DrawWindow("Memory View", cpu.ram.data(), 4096, 0);
  ImGui::PopFont();

  EndFrame();
}

void Chipbit::Gui::SetImGuiStyle() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  /// 0 = FLAT APPEARENCE
  /// 1 = MORE "3D" LOOK
  int is3D = 1;

  colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
  colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
  colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

  style.PopupRounding = 3;

  style.WindowPadding = ImVec2(4, 4);
  style.FramePadding = ImVec2(6, 4);
  style.ItemSpacing = ImVec2(6, 2);

  style.ScrollbarSize = 18;

  style.WindowBorderSize = 1;
  style.ChildBorderSize = 1;
  style.PopupBorderSize = 1;
  style.FrameBorderSize = (float)is3D;

  style.WindowRounding = 3;
  style.ChildRounding = 3;
  style.FrameRounding = 3;
  style.ScrollbarRounding = 2;
  style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK
  style.TabBorderSize = (float)is3D;
  style.TabRounding   = 3;

  colors[ImGuiCol_DockingEmptyBg]     = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
  colors[ImGuiCol_Tab]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_TabHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_TabActive]          = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
  colors[ImGuiCol_TabUnfocused]       = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
  colors[ImGuiCol_DockingPreview]     = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
#endif
}