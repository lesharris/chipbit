#pragma once

#include <map>
namespace Chipbit {
  enum class Action {
    K1, K2, K3, KC,
    K4, K5, K6, KD,
    K7, K8, K9, KE,
    KA, K0, KB, KF,
    Pause
  };

  inline std::map<Action, unsigned char> ActionToKey{
      {Action::K0, 0}, {Action::K1, 1}, {Action::K2, 2}, {Action::K3, 3},
      {Action::K4, 4}, {Action::K5, 5}, {Action::K6, 6}, {Action::K7, 7},
      {Action::K8, 8}, {Action::K9, 9}, {Action::KA, 0xA}, {Action::KB, 0xB},
      {Action::KC, 0xC}, {Action::KD, 0xD}, {Action::KE, 0xE}, {Action::KF, 0xF}
  };

  enum class ActionSource {
    Keyboard,
    Mouse
  };
}