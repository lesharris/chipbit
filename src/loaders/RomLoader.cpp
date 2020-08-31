#include "RomLoader.h"

#include <fstream>

std::vector<unsigned char> Chipbit::RomLoader::load(const std::string &romPath) {
  std::ifstream romFile;
  romFile.open(romPath, std::ofstream::binary);

  romFile.seekg(0, romFile.end);
  auto length = (unsigned long) romFile.tellg();
  romFile.seekg(0, romFile.beg);

  auto content = new char[length];

  romFile.read(content, length);
  romFile.close();

  return std::vector<unsigned char>(content, content + length);
}
