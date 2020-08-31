#include "Chip8.h"

#include <vector>

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


  m_Vectors = {
      OPCODE(0000), OPCODE(1000), OPCODE(2000), OPCODE(3000),
      OPCODE(4000), OPCODE(5000), OPCODE(6000), OPCODE(7000),
      OPCODE(8000), OPCODE(9000), OPCODE(A000), OPCODE(B000),
      OPCODE(C000), OPCODE(D000), OPCODE(E000), OPCODE(F000)
  };

  std::copy(m_Font.begin(), m_Font.end(), m_CPU->ram.begin());

  //m_OnColor = Color(255, 255, 255, 255);
 // m_OffColor = Color(33, 33, 33,255);

  m_OnColor = Color(183, 109, 104, 255);
  m_OffColor = Color(27, 36, 50, 255);

  m_CPU->framebuffer = std::vector<unsigned int>(64 * 32, m_OffColor);
}

bool Chipbit::Chip8::Tick() {
  if(m_CPU->PC >= 4096) {
    m_CPU->PC = 0x200;
    CB_WARN("Program Counter overflow");
  }

  if(m_CPU->waitForKeypress) {
    auto index = 0;
    for(auto k : m_CPU->keys) {
      if(k == 1) {
        m_CPU->waitForKeypress = false;
        break;
      }
      index++;
    }

    if(m_CPU->waitForKeypress)
      return false;

    V(m_CPU->keypressResultReg) = index;
  }

  unsigned short opcode = m_CPU->ram[m_CPU->PC] << 8 | m_CPU->ram[m_CPU->PC + 1];
  m_CPU->PC += 2;

  m_Vectors[(opcode & 0xF000) >> 12](opcode & 0x0FFF);

  return true;
}

void Chipbit::Chip8::Opcode0000(unsigned short operand) {
  switch(operand) {
    case 0x0E0:
      m_CPU->framebuffer = std::vector<unsigned int>(64 * 32, m_OffColor);
      m_CPU->draw = true;
      break;

    case 0x0EE:
      m_CPU->sp--;
      m_CPU->PC = m_CPU->stack[m_CPU->sp];
      break;

    default:
      CB_WARN("Invalid 0x0000 Opcode: {:#04x}", operand);
      break;
  }
}

void Chipbit::Chip8::Opcode1000(unsigned short operand) {
  m_CPU->PC = operand;
}

void Chipbit::Chip8::Opcode2000(unsigned short operand) {
  m_CPU->stack[m_CPU->sp++] = m_CPU->PC;
  m_CPU->PC = operand;
}

void Chipbit::Chip8::Opcode3000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char val = operand & 0x0FF;

  if(V(X) == val) {
    m_CPU->PC += 2;
  }
}

void Chipbit::Chip8::Opcode4000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char val = operand & 0x0FF;

  if(V(X) != val) {
    m_CPU->PC += 2;
  }
}

void Chipbit::Chip8::Opcode5000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char Y = (operand & 0x0F0) >> 4;

  if(V(X) == V(Y)) {
    m_CPU->PC += 2;
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

  switch(operand & 0x00F) {
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
      V(0xF) = V(X) > (0xFF - V(Y));
      V(X) += V(Y);
      break;

    case 0x5:
      V(0xF) = V(X) > V(Y);
      V(X) -= V(Y);
      break;

    case 0x6:
      V(0xF) = V(X) & 1;
      V(X) >>= 1;
      // V(X) = V(Y) >> 1 ;
      break;

    case 0x7:
      V(0xF) = V(Y) > V(X);
      V(X) = V(Y) - V(X);
      break;

    case 0xE:
      V(0xF) = V(X) & 0x80 ? 1 : 0;
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

  if(V(X) != V(Y)) {
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

  for(auto h = 0; h < height; h++) {
    unsigned char sprite = m_CPU->ram[m_CPU->I + h];

    for(auto w = 0; w < 8; w++) {
      if ((sprite & (0x80 >> w)) != 0) {
        const int idx = (V(X) + w + ((V(Y) + h) * 64));

        int curState = (m_CPU->framebuffer[idx] == m_OnColor);
        if (curState == 1)
          V(0xF) = 1;

        const int result = (curState ^= 1);
        m_CPU->framebuffer[idx] = (result) ? m_OnColor : m_OffColor;
      }
    }
  }

  m_CPU->draw = true;
}

void Chipbit::Chip8::OpcodeE000(unsigned short operand) {
  unsigned char X = (operand & 0xF00) >> 8;
  unsigned char operation = operand & 0x0FF;

  switch(operation) {
    case 0x9E:
      if(m_CPU->keys[V(X)] == 1)
        m_CPU->PC += 2;
      break;

    case 0xA1:
      if(m_CPU->keys[V(X)] == 0)
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

  switch(operation) {
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
     // V(0xF) = (m_CPU->I + (unsigned short)V(X)) > 0xFFF;
      m_CPU->I += V(X);
      break;

    case 0x29:
      m_CPU->I = 0x0 + (5 * V(X));
      break;

    case 0x33:
      m_CPU->ram[m_CPU->I] = V(X) / 100;
      m_CPU->ram[m_CPU->I + 1] = (V(X) / 10) % 10;
      m_CPU->ram[m_CPU->I + 2] = V(X) % 10;
      break;

    case 0x55:
      for(int i = 0; i <= X; i++)
        m_CPU->ram[m_CPU->I + i] = V(i);
      break;

    case 0x65:
      for(int i = 0; i <= X; i++)
        V(i) = m_CPU->ram[m_CPU->I + i];
      break;

    default:
      CB_WARN("Invalid 0xF000 Operand: {:#04x}", operand);
      break;
  }
}

void Chipbit::Chip8::Load(const std::vector<unsigned char> &data, int address) {
  if(address >= 4096) {
    return;
  }

  std::copy(data.begin(), data.end(), m_CPU->ram.begin() + address);
}

unsigned int Chipbit::Chip8::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
  return r << 24 | g << 16 | b << 8 | a;
}


