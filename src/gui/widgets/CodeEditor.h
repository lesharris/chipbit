#pragma once

#include "Widget.h"
#include <TextEditor.h>

namespace Chipbit {
  class CodeEditor : public Widget {
  public:
    CodeEditor();
    void Render(Chip8::CPU &cpu) override;
  private:
    TextEditor m_Editor{};
  };
}