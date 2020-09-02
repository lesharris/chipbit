#include "CodeEditor.h"

Chipbit::CodeEditor::CodeEditor() {
  m_Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
  m_Editor.SetText("");
}

void Chipbit::CodeEditor::Render(Chipbit::Chip8::CPU &cpu) {
  ImGui::Begin("Code Editor");
  m_Editor.Render("TextEditor");
  ImGui::End();
}

