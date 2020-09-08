#include "Utils.h"

std::vector<std::string> Chipbit::StringUtils::SplitLines(const std::string &text) {
  std::vector<std::string> lines;

  auto startPos = 0;
  int endPos = text.find('\n');

  if (endPos == -1) {
    lines.push_back(text);
    return lines;
  }

  while (endPos != std::string::npos) {
    lines.push_back(text.substr(startPos, endPos - startPos));
    startPos = endPos + 1;

    endPos = text.find('\n', startPos);
  }

  lines.push_back(text.substr(startPos,
                              std::min(endPos, static_cast<int>(text.size())) -
                              startPos + 1));

  return lines;
}
