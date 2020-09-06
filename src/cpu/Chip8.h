#pragma once

#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <functional>

namespace Chipbit {
  class Chip8 {
  public:
    struct CPU {
      CPU() {
        Reset();
      }

      void Reset() {
        registers = std::vector<unsigned char>(16, 0);
        userRpl = std::vector<unsigned char>(16, 0);
        ram = std::vector<unsigned char>(128 * 1024, 0);
        keys = std::vector<unsigned char>(16, 0);
        layers = {
            std::vector<unsigned char>(128 * 64, 0),
            std::vector<unsigned char>(128 * 64, 0)
        };

        sound_timer = 0;
        delay_timer = 0;

        draw = false;
        waitForKeypress = false;

        I = 0;
        PC = 0x200;
        SP = 0;

        for(unsigned short & i : stack)
          i = 0;

        hires = false;
        c8hires = false;
        activePlanes = 1;

        halted = false;
      }

      unsigned char GetRandom() {
        std::uniform_int_distribution<> dist(0, 255);

        return static_cast<unsigned char>(dist(mt));
      }

      bool draw = false;
      bool waitForKeypress = false;
      unsigned char keypressResultReg = 0;

      unsigned char sound_timer = 0;
      unsigned char delay_timer = 0;

      unsigned short I = 0;
      unsigned short PC = 0x200;

      std::vector<unsigned char> registers;
      std::vector<unsigned char> userRpl;
      std::vector<unsigned char> keys;
      std::vector<unsigned char> ram;
      std::vector<std::vector<unsigned char>> layers;

      unsigned char SP = 0;
      unsigned short stack[16]{};

      std::random_device rd;
      std::mt19937 mt{rd()};

      bool hires = false;
      bool c8hires = false;
      bool halted = false;
      unsigned char activePlanes = 1;
    };

  public:
    Chip8();

    bool Tick();
    void Load(const std::vector<unsigned char>& data, int address);
    [[nodiscard]] bool Draw() const { return m_CPU->draw; }
    void ClearDraw() { m_CPU->draw = false; }
    void ResetKeys() { m_CPU->keys = std::vector<unsigned char>(16, 0); }
    void SetKeys(const std::vector<unsigned char>& keys) { m_CPU->keys = keys; }
    void TickTimers() {
      if(m_CPU->delay_timer != 0)
        m_CPU->delay_timer--;

      if(m_CPU->sound_timer != 0)
        m_CPU->sound_timer--;
    }
    void Reset() {
      m_CPU->Reset();
      LoadFonts();
    }

    CPU& Get() { return *m_CPU; }
    [[nodiscard]] std::vector<std::vector<unsigned char>> Framebuffer() const { return m_CPU->layers; }
  private:
    void Opcode0000(unsigned short operand);
    void Opcode1000(unsigned short operand);
    void Opcode2000(unsigned short operand);
    void Opcode3000(unsigned short operand);
    void Opcode4000(unsigned short operand);
    void Opcode5000(unsigned short operand);
    void Opcode6000(unsigned short operand);
    void Opcode7000(unsigned short operand);
    void Opcode8000(unsigned short operand);
    void Opcode9000(unsigned short operand);
    void OpcodeA000(unsigned short operand);
    void OpcodeB000(unsigned short operand);
    void OpcodeC000(unsigned short operand);
    void OpcodeD000(unsigned short operand);
    void OpcodeE000(unsigned short operand);
    void OpcodeF000(unsigned short operand);

    static unsigned int Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a);

    void ClearRow(int row, int layer);
    void ClearColumn(int column, int layer);
    void CopyRow(int source, int destination, int layer);
    void CopyColumn(int source, int destination, int layer);

    void ScrollUp(int count, int layer);
    void ScrollDown(int count, int layer);
    void ScrollLeft(int count, int layer);
    void ScrollRight(int count, int layer);

    bool SetPixel(int x, int y, int currentPixel, int layer);

    void LoadFonts();

  private:
    std::vector<unsigned char> m_Font;
    std::vector<unsigned char> m_BigFont;
    std::vector<std::function<void(unsigned short)>> m_Vectors;
    std::shared_ptr<CPU> m_CPU;

    unsigned int m_OnColor = 0;
    unsigned int m_OffColor = 0;
  };
}