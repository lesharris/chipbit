#pragma once

// C++ implemention of John Earnest's Octo compiler: https://github.com/JohnEarnest/Octo

#include <string>
#include <utility>
#include <variant>
#include <map>
#include <vector>
#include <tuple>
#include <functional>
#include <cmath>

#include "../core/Utils.h"

#define Unary(label, intFunc, floatFunc) \
  UnaryPair{(label), [](auto& x) -> void {std::visit(overload{(intFunc),(floatFunc)}, x);}}

namespace Chipbit {
  class Compiler {
  public:
    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class... Ts> overload(Ts...) -> overload<Ts...>;

    template<typename T, typename U>
    using UnaryFunc = std::function<void(std::variant<T, U>&)>;
    using UnaryPair = std::pair<std::string, UnaryFunc<int, float>>;

    class DebugInfo {
    public:
      explicit DebugInfo(const std::string& source) : lines(StringUtils::SplitLines(source)) {};

      void mapAddr(int addr, int pos) {
        this->_locs[addr] = pos;
      }

      int getLine(int addr) {
        if(_locs.find(addr) != _locs.end()) {
          return posToLine(_locs[addr]);
        }

        return -1;
      }

      int getAddr(int line) {
        for(auto item : _locs) {
          auto addr = item.first;

          if(posToLine(_locs[addr]) == line)
            return addr;
        }

        return -1;
      }

    private:
      int posToLine(int pos) {
        int i;
        for(i = 0; i < lines.size(); i++) {
          pos -= (int)lines[i].length() + 1;
          if(pos <= 0)
            break;
        }

        return i;
      }
    private:
      std::vector<std::string> lines{};
      std::map<int, int> _locs{};
    };

    class CompilerException : public std::exception {
    public:
      explicit CompilerException(std::string msg) : m_Msg(std::move(msg)) {}

      [[nodiscard]] std::string GetMsg() const { return m_Msg; }

    private:
      std::string m_Msg;
    };

  public:
    explicit Compiler(const std::string& source) : dbginfo(DebugInfo(source)) {};

  private:
    std::variant<int, std::string> parse(const std::string &token);

    std::tuple<bool, int> parseNumber(const std::string &token);

    std::vector<std::tuple<std::variant<int, std::string>, int, int>> tokenize(const std::string &text);

  private:
    std::map<char, char> escapeChars{
        {'t',  '\t'},
        {'n',  '\n'},
        {'r',  '\r'},
        {'v',  '\v'},
        {'0',  '\0'},
        {'\\', '\\'},
        {'"',  '"'},
    };

    std::map<std::string, UnaryFunc<int, float>> unaryFuncs{
      Unary("-", [](int& i) { i = -i; }, [](float& f) { f = -f; }),
      Unary("~", [](int& i) { i = ~i;}, [](float& f) {}),
      Unary("!", [](int& i) { i = !i;}, [](float& f) {f = !f;}),
      Unary("sin", [](int& i) { i = static_cast<int>(std::sin(i));}, [](float& f){f = std::sin(f);}),
      Unary("cos", [](int& i) { i = static_cast<int>(std::cos(i));}, [](float& f){f = std::cos(f);}),
      Unary("tan", [](int& i) { i = static_cast<int>(std::tan(i));}, [](float& f){f = std::tan(f);}),
      Unary("exp", [](int& i) { i = static_cast<int>(std::exp(i));}, [](float& f){f = std::exp(f);}),
      Unary("log", [](int& i) { i = static_cast<int>(std::log(i));}, [](float& f){f = std::log(f);}),
      Unary("abs", [](int& i) { i = static_cast<int>(std::abs(i));}, [](float& f){f = std::abs(f);}),
      Unary("sqrt", [](int& i) { i = static_cast<int>(std::sqrt(i));}, [](float& f){f = std::sqrt(f);}),
      Unary("sign", [](int& i) { i = static_cast<int>(std::exp(i));}, [](float& f){f = std::exp(f);}),
      Unary("ceil", [](int& i) { i = static_cast<int>(std::ceil(i));}, [](float& f){f = std::floor(f);}),
      Unary("floor", [](int& i) { i = static_cast<int>(std::floor(i));}, [](float& f){f = std::floor(f);})


    };

    /*        {"~", [](auto x) -> auto {return ~x;}},
        {"!", [](auto x) -> auto {return !x;}}*/

    std::map<std::string, std::function<float(float x)>> unaryFloatFuncs {
        {"sin", [](float x) -> float {return std::sin(x);}},
        {"cos", [](float x) -> float {return std::cos(x);}},
        {"tan", [](float x) -> float {return std::tan(x);}},
        {"exp", [](float x) -> float {return std::exp(x);}},
        {"log", [](float x) -> float {return std::log(x);}},
        {"abs", [](float x) -> float {return std::abs(x);}},
        {"sqrt", [](float x) -> float {return std::sqrt(x);}},
        {"sign", [](float x) -> float {return x >= 0.0f ? 1 : -1.0f;}},
        {"ceil", [](float x) -> float {return std::ceil(x);}},
        {"floor", [](float x) -> float {return std::floor(x);}}
    };

    std::map<std::string, std::function<int(int x, std::vector<int>& m)>> unaryMiscFuncs {
        {"@", [](int x, std::vector<int>& m) -> int {return m[(0 | x) - 0x200] || false;}}
    };

    std::map<std::string, std::function<int(int x, int y)>> binaryIntFuncs {
        {"-", [](int x, int y) -> int {return x - y;}},
        {"+", [](int x, int y) -> int {return x + y;}},
        {"*", [](int x, int y) -> int {return x * y;}},
        {"%", [](int x, int y) -> int {return x % y;}},
        {"&", [](int x, int y) -> int {return x & y;}},
        {"|", [](int x, int y) -> int {return x | y;}},
        {"^", [](int x, int y) -> int {return x ^ y;}},
        {"<<", [](int x, int y) -> int {return x << y;}},
        {">>", [](int x, int y) -> int {return x >> y;}},
        {"pow", [](int x, int y) -> int {return std::pow(x, y);}},
        {"min", [](int x, int y) -> int {return std::min(x, y);}},
        {"max", [](int x, int y) -> int {return std::max(x, y);}},
        {"<", [](int x, int y) -> int {return x < y;}},
        {">", [](int x, int y) -> int {return x > y;}},
        {"<=", [](int x, int y) -> int {return x <= y;}},
        {">=", [](int x, int y) -> int {return x >= y;}},
        {"==", [](int x, int y) -> int {return x == y;}},
        {"!=", [](int x, int y) -> int {return x != y;}},
    };

    std::map<std::string, std::function<float(float x, float y)>> binaryFloatFuncs {
        {"/", [](float x, float y) -> float {return x / y;}}
    };

    std::vector<int> rom{};
    DebugInfo dbginfo;
    std::vector<std::tuple<int, int>> loops{};
    std::vector<std::tuple<int, int, std::string>> branches{};
    std::vector<int> stack{};
    std::map<std::string, int> dict{};
    std::map<std::string, std::vector<int>> protos{};
    std::map<std::string, bool> longproto{};
    std::map<std::string, int> aliases{};
    std::map<std::string, int> constants {
        {"OCTO_KEY_1", 0x1},
        {"OCTO_KEY_2", 0x2},
        {"OCTO_KEY_3", 0x3},
        {"OCTO_KEY_4", 0xC},
        {"OCTO_KEY_Q", 0x4},
        {"OCTO_KEY_W", 0x5},
        {"OCTO_KEY_E", 0x6},
        {"OCTO_KEY_R", 0xD},
        {"OCTO_KEY_A", 0x7},
        {"OCTO_KEY_S", 0x8},
        {"OCTO_KEY_D", 0x9},
        {"OCTO_KEY_F", 0xE},
        {"OCTO_KEY_Z", 0xA},
        {"OCTO_KEY_X", 0x0},
        {"OCTO_KEY_C", 0xB},
        {"OCTO_KEY_V", 0xF},
    };
  };
}