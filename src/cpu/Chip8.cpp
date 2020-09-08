#include "Chip8.h"

#include <vector>
#include <algorithm>
#include <sstream>
#include "../core/events/Events.h"
#include "../core/EventManager.h"
#include "../core/Log.h"

#define V(R)  m_CPU->registers[R]
#define OPCODE(CODE) [this](unsigned short operand) { this->Opcode##CODE(operand); }

Chipbit::Chip8::Chip8() {
  m_CPU = std::make_shared<CPU>();

  m_Font = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  m_BigFont = {
      0x7C, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7C, 0x00, // 0
      0x08, 0x18, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3C, 0x00, // 1
      0x7C, 0x82, 0x02, 0x02, 0x04, 0x18, 0x20, 0x40, 0xFE, 0x00, // 2
      0x7C, 0x82, 0x02, 0x02, 0x3C, 0x02, 0x02, 0x82, 0x7C, 0x00, // 3
      0x84, 0x84, 0x84, 0x84, 0xFE, 0x04, 0x04, 0x04, 0x04, 0x00, // 4
      0xFE, 0x80, 0x80, 0x80, 0xFC, 0x02, 0x02, 0x82, 0x7C, 0x00, // 5
      0x7C, 0x82, 0x80, 0x80, 0xFC, 0x82, 0x82, 0x82, 0x7C, 0x00, // 6
      0xFE, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20, 0x20, 0x00, // 7
      0x7C, 0x82, 0x82, 0x82, 0x7C, 0x82, 0x82, 0x82, 0x7C, 0x00, // 8
      0x7C, 0x82, 0x82, 0x82, 0x7E, 0x02, 0x02, 0x82, 0x7C, 0x00, // 9
      0x10, 0x28, 0x44, 0x82, 0x82, 0xFE, 0x82, 0x82, 0x82, 0x00, // A
      0xFC, 0x82, 0x82, 0x82, 0xFC, 0x82, 0x82, 0x82, 0xFC, 0x00, // B
      0x7C, 0x82, 0x80, 0x80, 0x80, 0x80, 0x80, 0x82, 0x7C, 0x00, // C
      0xFC, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0xFC, 0x00, // D
      0xFE, 0x80, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80, 0xFE, 0x00, // E
      0xFE, 0x80, 0x80, 0x80, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x00, // F
  };

  m_Vectors = {
      OPCODE(0000), OPCODE(1000), OPCODE(2000), OPCODE(3000),
      OPCODE(4000), OPCODE(5000), OPCODE(6000), OPCODE(7000),
      OPCODE(8000), OPCODE(9000), OPCODE(A000), OPCODE(B000),
      OPCODE(C000), OPCODE(D000), OPCODE(E000), OPCODE(F000)
  };

  LoadFonts();

  m_OnColor = Color(255, 255, 255, 255);
  m_OffColor = Color(33, 33, 33, 255);

  m_CPU->layers = {
      std::vector<unsigned char>(128 * 64, 0),
      std::vector<unsigned char>(128 * 64, 0)
  };
}

bool Chipbit::Chip8::Tick() {

  if(m_CPU->halted)
    return false;

  if (m_CPU->PC >= 128 * 1024) {
    m_CPU->PC = 0x200;
    CB_WARN("Program Counter overflow");
  }

  if (m_CPU->waitForKeypress) {
    auto index = 0;

    for (auto k : m_CPU->keys) {
      if (k == 1) {
        m_CPU->waitForKeypress = false;
        break;
      }
      index++;
    }

    if (m_CPU->waitForKeypress)
      return false;

    V(m_CPU->keypressResultReg) = index;
  }

  unsigned short opcode = m_CPU->ram[m_CPU->PC] << 8 | m_CPU->ram[m_CPU->PC + 1];

  // Initialize Chip8 Hires
  if (m_CPU->PC == 0x200 && opcode == 0x1260) {
    opcode = 0x12C0;
    m_CPU->c8hires = true;
  }

  m_CPU->PC += 2;

  m_Vectors[(opcode & 0xF000) >> 12](opcode & 0x0FFF);

  return true;
}

void Chipbit::Chip8::Opcode0000(unsigned short operand) {
  if (operand == 0x0230) { // Chip8 Hires Clearscreen
    m_CPU->layers[0] = std::vector<unsigned char>(128 * 64, 0);
    m_CPU->draw = true;
  } else {
    switch ((operand & 0x0F0) >> 4) {
      case 0xC: {
        for(auto layer = 0; layer < 2; layer++) {
          if((m_CPU->activePlanes & (layer + 1)) == 0)
            continue;
          auto count = operand & 0x00F;
          ScrollDown(count, layer);
        }
        m_CPU->draw = true;
      }
        break;

      case 0xD: {
        for(auto layer = 0; layer < 2; layer++) {
          if ((m_CPU->activePlanes & (layer + 1)) == 0)
            continue;
          auto count = operand & 0x00F;
          ScrollUp(count, layer);
        }
        m_CPU->draw = true;
      }
        break;

      case 0xE:
        switch (operand) {
          case 0x0E0:
            for(auto layer = 0; layer < 2; layer++) {
              if ((m_CPU->activePlanes & (layer + 1)) == 0)
                continue;

              m_CPU->layers[layer] = std::vector<unsigned char>(128 * 64, 0);
            }
            m_CPU->draw = true;
            break;

          case 0x0EE:
            if (m_CPU->SP == 0) {
              CB_ERROR("Stack underflow");
            } else {
              m_CPU->SP--;
              m_CPU->PC = m_CPU->stack[m_CPU->SP];
            }
            break;

          default:
            CB_WARN("Invalid 0x00E0 Opcode: {:#04x}", operand);
            break;
        }
        break;

      case 0xF:
        switch (operand) {
          case 0x0FB:
            for(auto layer = 0; layer < 2; layer++) {
              if ((m_CPU->activePlanes & (layer + 1)) == 0)
                continue;
              ScrollRight(4, layer);
            }
            m_CPU->draw = true;
            break;

          case 0x0FC:
            for(auto layer = 0; layer < 2; layer++) {
              if ((m_CPU->activePlanes & (layer + 1)) == 0)
                continue;
              ScrollLeft(4, layer);
            }
            m_CPU->draw = true;
            break;

          case 0x0FD:
            CB_INFO("Exit");
            m_CPU->halted = true;
            EventManager::Dispatcher().trigger<Events::PauseEvent>();
            break;

          case 0x0FE:
            CB_INFO("Lores On");
            m_CPU->hires = false;
            break;

          case 0x0FF:
            CB_INFO("Hires On");
            m_CPU->hires = true;
            break;

          default:
            CB_WARN("Invalid 0x00F0 Opcode: {:#04x}", operand);
            break;
        }
        break;

      default:
        CB_WARN("Invalid 0x0000 Opcode: {:#04x}", operand);
        break;
    }
  }
}

void Chipbit::Chip8::Opcode1000(unsigned short operand) {
  m_CPU->PC = operand;
}

void Chipbit::Chip8::Opcode2000(unsigned short operand) {
  m_CPU->stack[m_CPU->SP] = m_CPU->PC;

  if (m_CPU->SP < 15) {
    m_CPU->SP++;
  } else {
    CB_ERROR("Stack overflow");
  }

  m_CPU->PC = operand;
}

void Chipbit::Chip8::Opcode3000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char val = operand & 0x0FF;

  if (V(X) == val) {
    m_CPU->PC += 2;
  }
}

void Chipbit::Chip8::Opcode4000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char val = operand & 0x0FF;

  if (V(X) != val) {
    m_CPU->PC += 2;
  }
}

void Chipbit::Chip8::Opcode5000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char Y = (operand & 0x0F0) >> 4;
  unsigned char operation = (operand & 0x00F);

  switch (operation) {
    case 0:
      if (V(X) == V(Y)) {
        m_CPU->PC += 2;
      }
      break;

    case 2: {
      unsigned int dist = std::abs(X - Y);

      if(X < Y) {
        for(auto z = 0; z <= dist; z++)
            m_CPU->ram[m_CPU->I + z] = V(X + z);
      } else {
        for(auto z = 0; z <= dist; z++)
          m_CPU->ram[m_CPU->I + z] = V(X - z);
      }
    }
      break;

    case 3: {
      unsigned int dist= std::abs(X - Y);

      if(X < Y) {
        for(auto z = 0; z <= dist; z++)
          V(X + z) = m_CPU->ram[m_CPU->I + z];
      } else {
        for(auto z = 0; z <= dist; z++)
          V(X - z) = m_CPU->ram[m_CPU->I + z];
      }
    }
      break;

    default:
      CB_WARN("Invalid 0x5000 opcode {0}", operand);
      break;
  }
}

void Chipbit::Chip8::Opcode6000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char val = operand & 0x0FF;

  V(X) = val;
}

void Chipbit::Chip8::Opcode7000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char val = operand & 0x0FF;

  V(X) += val;
}

void Chipbit::Chip8::Opcode8000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char Y = (operand & 0x0F0) >> 4;

  switch (operand & 0x00F) {
    case 0x0:
      V(X) = V(Y);
      break;

    case 0x1:
      V(X) |= V(Y);
      break;

    case 0x2:
      V(X) &= V(Y);
      break;

    case 0x3:
      V(X) ^= V(Y);
      break;

    case 0x4:
      V(0xF) = V(X) >= (0xFF - V(Y));
      V(X) += V(Y);
      break;

    case 0x5:
      V(0xF) = V(X) >= V(Y);
      V(X) -= V(Y);

      if (V(X) == 0)
        V(0xF) = 1;
      break;

    case 0x6:
      V(0xF) = V(X) & 1;
      V(X) >>= 1;
      // V(X) = V(Y) >> 1 ;
      break;

    case 0x7:
      V(0xF) = V(Y) >= V(X);
      V(X) = V(Y) - V(X);
      break;

    case 0xE:
      V(0xF) = ((V(X) >> 7) & 0x1);
      V(X) <<= 1;
      // V(X) = V(Y) << 1 ;
      break;

    default:
      CB_WARN("Invalid 0x8000 range opcode: {:#04x}", operand);
      break;
  }
}

void Chipbit::Chip8::Opcode9000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char Y = (operand & 0x0F0) >> 4;

  if (V(X) != V(Y)) {
    m_CPU->PC += 2;
  }
}

void Chipbit::Chip8::OpcodeA000(unsigned short operand) {
  m_CPU->I = operand;
}

void Chipbit::Chip8::OpcodeB000(unsigned short operand) {
  m_CPU->PC = operand + V(0);
}

void Chipbit::Chip8::OpcodeC000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char mask = operand & 0x0FF;

  V(X) = m_CPU->GetRandom() & mask;
}

void Chipbit::Chip8::OpcodeD000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char Y = (operand & 0x0F0) >> 4;
  unsigned char height = operand & 0x00F;

  V(0xF) = 0;
  auto scale = m_CPU->hires ? 1 : 2;
  auto I = m_CPU->I;

  auto rowSize = 64 * scale;
  auto colSize = 32 * scale;

  for(auto layer = 0; layer < 2; layer++ ) {
    if((m_CPU->activePlanes & (layer + 1)) == 0)
      continue;

    switch (height) {
      case 0x0:
   //     if (m_CPU->hires) {
          for (auto h = 0; h < 16; h++) {
            unsigned short spriteRow = m_CPU->ram[I + 2 * h] << 8 | m_CPU->ram[I + 2 * h + 1];
            for (auto w = 0; w < 16; w++) {
              unsigned char pixel = ((spriteRow >> (15 - w)) & 0x1) != 0;
              int xw = V(X) + w;
              int yh = V(Y) + h;

              if(!pixel)
                continue;

              if(SetPixel(xw, yh, pixel, layer))
                V(0xF) = 1;
            }
          }

          I += 32;
     /*   } else {
          for (auto h = 0; h < 16; h++) {
            for (auto w = 0; w < 8; w++) {
              int pixel = (m_CPU->ram[I + h] >> (7 - w)) & 1;

              const int xw = (V(X) * scale) + w * scale % (64 * scale);
              const int yh = (V(Y) * scale) + h * scale % (32 * scale);

              auto result = SetPixel(xw, yh, pixel, layer);

              if (result)
                V(0xF) = 1;
            }
          }
          I += 24;
        }*/
        break;

      default:
        for (auto h = 0; h < height; h++) {
          for (auto w = 0; w < 8; w++) {
            const int xw = (V(X) * scale) + w * scale % rowSize;
            const int yh = (V(Y) * scale) + h * scale % colSize;

            unsigned char pixel = ((m_CPU->ram[I + h] >> (7 - w)) & 0x1) != 0;

            if(!pixel)
              continue;

            if(SetPixel(xw, yh, pixel, layer))
              V(0xF) = 1;
          }
        }
        I += height;
        break;
    }
  }

  m_CPU->draw = true;
}

void Chipbit::Chip8::OpcodeE000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char operation = operand & 0x0FF;

  std::stringstream ss;

  for(int i = 0; i < 16; i++)
    if(m_CPU->keys[i] != 0)
      ss << fmt::format("{:02x}", i);

  CB_INFO("Keys: {0}", ss.str());
  CB_INFO("X: {0}, V(X): {1}, m_CPU->keys[V(X)]: {2}", X, V(X), m_CPU->keys[V(X)]);
  switch (operation) {
    case 0x9E:
      if (m_CPU->keys[V(X)])
        m_CPU->PC += 2;
      break;

    case 0xA1:
      if (!m_CPU->keys[V(X)])
        m_CPU->PC += 2;
      break;

    default:
      CB_WARN("Invalid 0xF000 Operand: {0}", operand);
      break;
  }
}

void Chipbit::Chip8::OpcodeF000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char operation = operand & 0x0FF;

  switch (operand) {
    // Planes
    case 0x001:
    case 0x101:
    case 0x201:
    case 0x301:
      m_CPU->activePlanes = X & 0x3;
      break;

    default: {
      switch (operation) {
        case 0x0:
          m_CPU->I = m_CPU->ram[m_CPU->PC] << 8 | m_CPU->ram[m_CPU->PC + 1] & 0xFFFF;
          m_CPU->PC += 2;
          break;

        case 0x2:
          for(auto z = 0; z < 16; z++)
            m_CPU->pattern[z] = m_CPU->ram[m_CPU->I + z];
          break;

        case 0x7:
          V(X) = m_CPU->delay_timer;
          break;

        case 0xA:
          m_CPU->waitForKeypress = true;
          m_CPU->keypressResultReg = X;
          break;

        case 0x15:
          m_CPU->delay_timer = V(X);
          break;

        case 0x18:
          m_CPU->sound_timer = V(X);
          break;

        case 0x1E:
          V(0xF) = (m_CPU->I + (unsigned short)V(X)) > 0xFFF;
          m_CPU->I += V(X);
          break;

        case 0x29:
          m_CPU->I = 5 * V(X);
          break;

        case 0x30:
          m_CPU->I = m_Font.size() + (10 * V(X));
          break;

        case 0x33:
          m_CPU->ram[m_CPU->I] = V(X) / 100;
          m_CPU->ram[m_CPU->I + 1] = (V(X) / 10) % 10;
          m_CPU->ram[m_CPU->I + 2] = V(X) % 10;
          break;

        case 0x55:
          for (int i = 0; i <= X; i++)
            m_CPU->ram[m_CPU->I + i] = V(i);
          break;

        case 0x65:
          for (int i = 0; i <= X; i++)
            V(i) = m_CPU->ram[m_CPU->I + i];
          break;

        case 0x75:
          for (int i = 0; i <= X; i++)
            if (i <= 7)
              m_CPU->userRpl[i] = V(i);
          break;

        case 0x85:
          for (int i = 0; i <= X; i++)
            if (i <= 7)
              V(i) = m_CPU->userRpl[i];
          break;

        default:
          CB_WARN("Invalid 0xF000 Operand: {:#04x}", operand);
          break;
      }
    }
      break;
  }
}

void Chipbit::Chip8::Load(const std::vector<unsigned char> &data, int address) {
  std::copy(data.begin(), data.end(), m_CPU->ram.begin() + address);
}

unsigned int Chipbit::Chip8::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
  return r << 24 | g << 16 | b << 8 | a;
}

void Chipbit::Chip8::CopyRow(int source, int destination, int layer) {
  std::copy_n(m_CPU->layers[layer].cbegin() + source * 128, 128, m_CPU->layers[layer].begin() + destination * 128);
}

void Chipbit::Chip8::CopyColumn(int source, int destination, int layer) {
  for (int y = 0; y < 64; ++y) {
    auto rowOffset = y * 128;
    m_CPU->layers[layer][destination + rowOffset] = m_CPU->layers[layer][source + rowOffset];
  }
}

void Chipbit::Chip8::ClearRow(int row, int layer) {
  std::fill_n(m_CPU->layers[layer].begin() + row * 128, 128, 0);
}

void Chipbit::Chip8::ClearColumn(int column, int layer) {
  for (int y = 0; y < 64; ++y) {
    m_CPU->layers[layer][column + (y * 128)] = 0;
  }
}

void Chipbit::Chip8::ScrollDown(int count, int layer) {
  for (int y = 64 - count - 1; y >= 0; --y) {
    CopyRow(y, y + count, layer);
  }

  for (int y = 0; y < count; ++y) {
    ClearRow(y, layer);
  }
}

void Chipbit::Chip8::ScrollUp(int count, int layer) {
  for (int y = 0; y < (64 - count); ++y) {
    CopyRow(y + count, y, layer);
  }

  for (int y = 0; y < count; ++y) {
    ClearRow(64 - y - 1, layer);
  }
}

void Chipbit::Chip8::ScrollLeft(int count, int layer) {
  for (int x = 0; x < (128 - count); ++x) {
    CopyColumn(x + count, x, layer);
  }

  for (int x = 0; x < count; ++x) {
    ClearColumn(128 - x - 1, layer);
  }
}

void Chipbit::Chip8::ScrollRight(int count, int layer) {
  for (int x = 128 - count - 1; x >= 0; --x) {
    CopyColumn(x, x + count, layer);
  }

  for (int x = 0; x < count; ++x) {
    ClearColumn(x, layer);
  }
}

bool Chipbit::Chip8::SetPixel(int x, int y, int currentPixel, int layer) {
  x %= 128;
  y %= 64;

  std::vector<unsigned char>& fb = m_CPU->layers[layer];

  const int indexTL = (y * 128) + x;
  int indexTR = (y * 128) + x + 1;
  int indexBL = ((y + 1) * 128) + x;
  int indexBR = ((y + 1) * 128) + x + 1;

  if (indexTR >= fb.size())
    indexTR = indexTL;

  if (indexBL >= fb.size())
    indexBL = indexTL;

  if (indexBR >= fb.size())
    indexBR = indexTL;

  auto currentState = (fb[indexTL] == 1);
  fb[indexTL] = (currentState ^ currentPixel) ? 1 : 0;

  if (!m_CPU->hires) {
    fb[indexTR] = fb[indexTL];
    fb[indexBL] = fb[indexTL];
    fb[indexBR] = fb[indexTL];
  }

  return currentState & currentPixel;
}

void Chipbit::Chip8::LoadFonts() {
  std::copy(m_Font.begin(), m_Font.end(), m_CPU->ram.begin());
  std::copy(m_BigFont.begin(), m_BigFont.end(), m_CPU->ram.begin() + m_Font.size());
}


