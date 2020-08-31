#pragma once

#include <map>
#include <tuple>
#include <vector>

#include "Actions.h"

namespace Chipbit {
  class Input {
  public:
    Input();
    ~Input() = default;

    void addMapping(int key, Action action, ActionSource source);

    void removeMapping(int key);

    std::vector<Action> poll();

    enum ActionField {
      Sink = 0,
      Source
    };

  private:
    bool m_LeftPressed = false;
    bool m_RightPressed = false;
    std::map<int, std::tuple<Action, ActionSource>> m_mapping{};
  };
}