#pragma once

// C++ implemention of John Earnest's Octo compiler: https://github.com/JohnEarnest/Octo

#include <string>
#include <utility>
#include <variant>
#include <map>
#include <vector>
#include <tuple>

namespace Chipbit {
  class Compiler {
  public:

  class CompilerException : public std::exception {
  public:
    explicit CompilerException(std::string msg) : m_Msg(std::move(msg)) {}
    [[nodiscard]] std::string GetMsg() const { return m_Msg; }
  private:
    std::string m_Msg;
  };

  public:
    Compiler();

    void Test();
  private:
    std::variant<int, std::string> parse(const std::string& token);
    int parseNumber(const std::string& token);
    std::vector<std::tuple<std::variant<int, std::string>, int, int>> tokenize(const std::string& text);
  private:
    std::map<char, char> escapeChars{
        {'t', '\t'},
        {'n', '\n'},
        {'r', '\r'},
        {'v', '\v'},
        {'0', '\0'},
        {'\\', '\\'},
        {'"', '"'},
    };
  };
}