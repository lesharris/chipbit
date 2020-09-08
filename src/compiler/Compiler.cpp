#include "Compiler.h"

#include <regex>
#include <limits>

#include "../core/Log.h"

Chipbit::Compiler::Compiler() {

}

std::variant<int, std::string> Chipbit::Compiler::parse(const std::string &token) {
  auto num = parseNumber(token);

  if(std::isnan(num)) {
    return std::variant<int, std::string>(token);
  } else {
    return std::variant<int, std::string>(num);
  }
}

int Chipbit::Compiler::parseNumber(const std::string &token) {
  const auto flags = std::regex::flag_type::ECMAScript | std::regex::flag_type::icase;
  std::regex binaryNumber{"^[+\\-]?0b[01]+$", flags};
  std::regex hexNumber{"^[+\\-]?0x[0-9a-f]+$", flags};
  std::regex decimalNumber{"^[+\\-]?[0-9]+$", flags};

  if(std::regex_match(token, binaryNumber)) {
    std::string bitstring;
    bool isNegative = token.find('-') == 0;

    if(isNegative || token.find('+') == 0) {
      bitstring = token.substr(3, token.length() - 3);
    } else {
      bitstring = token.substr(2, token.length() - 2);
    }

    auto value = std::stoi(bitstring, nullptr, 2);
    return isNegative ? -value : value;
  }

  if(std::regex_match(token, hexNumber)) {
    return std::stoi(token, nullptr, 16);
  }

  if(std::regex_match(token, decimalNumber)) {
    return std::stoi(token);
  }

  return std::numeric_limits<int>::quiet_NaN();
}

void Chipbit::Compiler::Test() {
  auto foo = tokenize("0b101\n\"this is a string\"\n\someToken\nbooBear\n123\n0x20\nyay");
  CB_INFO("Tokenized");
}

std::vector<std::tuple<std::variant<int, std::string>, int, int>> Chipbit::Compiler::tokenize(const std::string &text) {
  std::vector<std::tuple<std::variant<int, std::string>, int, int>> ret{};
  int index = 0;
  std::string token;
  int tokenStart = -1;

  while(index < text.length()) {
    char c = text[index++];

    if(c == '#') {
      if (token.length() > 0) {
        ret.emplace_back(parse(token), tokenStart, index);
      }
      token.clear();
      while (c != '\n' && index < text.length())
        c = text[index++];
    } else if( c == '"') {
        if (token.length() > 0) {
          ret.emplace_back(parse(token), tokenStart, index);
        }

      std::string str;
      tokenStart = index;

      while (true) {
        if(index >= text.length()) {
          throw CompilerException("Missing a closing \" in a string literal.");
        }
        c = text[index++];

        if(c == '"') {
          break;
        }

        if(c == '\\') {
          char esc = text[index++];

          if(escapeChars.find(esc) == escapeChars.end()) {
            throw CompilerException(fmt::format("Unrecognized escape character '{0}' in a string literal.", esc));
          }
          c = escapeChars[esc];
        }
        str.push_back(c);
      }

      ret.emplace_back(std::variant<int, std::string>(str), tokenStart, index + 1);
      tokenStart = -1;
    } else if(c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\r') {
      if(token.length() > 0) {
        ret.emplace_back(parse(token), tokenStart, index );
        tokenStart = -1;
      }

      token.clear();
    } else {
      if(tokenStart == -1) {
        tokenStart = index;
      }

      token.push_back(c);
    }
  }

  if(token.length() > 0) {
    ret.emplace_back(parse(token), tokenStart, index + 1);
  }

  return ret;
}
