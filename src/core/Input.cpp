#include "Input.h"

#include "../Chipbit.h"

Chipbit::Input::Input() {
  addMapping(GLFW_KEY_1, Action::K1, ActionSource::Keyboard);
  addMapping(GLFW_KEY_2, Action::K2, ActionSource::Keyboard);
  addMapping(GLFW_KEY_3, Action::K3, ActionSource::Keyboard);
  addMapping(GLFW_KEY_4, Action::KC, ActionSource::Keyboard);
  addMapping(GLFW_KEY_Q, Action::K4, ActionSource::Keyboard);
  addMapping(GLFW_KEY_W, Action::K5, ActionSource::Keyboard);
  addMapping(GLFW_KEY_E, Action::K6, ActionSource::Keyboard);
  addMapping(GLFW_KEY_R, Action::KD, ActionSource::Keyboard);
  addMapping(GLFW_KEY_A, Action::K7, ActionSource::Keyboard);
  addMapping(GLFW_KEY_S, Action::K8, ActionSource::Keyboard);
  addMapping(GLFW_KEY_D, Action::K9, ActionSource::Keyboard);
  addMapping(GLFW_KEY_F, Action::KE, ActionSource::Keyboard);
  addMapping(GLFW_KEY_Z, Action::KA, ActionSource::Keyboard);
  addMapping(GLFW_KEY_X, Action::K0, ActionSource::Keyboard);
  addMapping(GLFW_KEY_C, Action::KB, ActionSource::Keyboard);
  addMapping(GLFW_KEY_V, Action::KF, ActionSource::Keyboard);
  addMapping(GLFW_KEY_ESCAPE, Action::Pause, ActionSource::Keyboard);
}

void Chipbit::Input::addMapping(int key, Action action, ActionSource source) {
  removeMapping(key);

  auto mapping = std::pair<int, std::tuple<Action, ActionSource>>{key, {action, source}};
  m_mapping.emplace(mapping);
}

void Chipbit::Input::removeMapping(int key) {
  auto pos = m_mapping.find(key);

  if(pos != m_mapping.end()) {
    m_mapping.erase(pos);
  }
}

std::vector<Chipbit::Action> Chipbit::Input::poll() {
  std::vector<Action> actions{};
  auto& app = Chipbit::Get();
  auto& window = app.GetWindow();

  for(auto& mapping : m_mapping) {
    auto key = mapping.first;
    auto action = std::get<Sink>(mapping.second);
    auto source = std::get<Source>(mapping.second);

    switch(source) {
      case ActionSource::Keyboard: {
        auto keyState = glfwGetKey(window, key);

        if(keyState == GLFW_PRESS) {
          actions.emplace_back(action);
        }
      }
        break;

      case ActionSource::Mouse: {
        auto mouseState = glfwGetMouseButton(window, key);

        // Debounce
        if(key == GLFW_MOUSE_BUTTON_LEFT) {
          if(mouseState == GLFW_PRESS && !m_LeftPressed) {
            m_LeftPressed = true;
            actions.emplace_back(action);
          } else {
            m_LeftPressed = mouseState == GLFW_PRESS;
          }
        }

        if(key == GLFW_MOUSE_BUTTON_RIGHT) {
          if(mouseState == GLFW_PRESS && !m_RightPressed) {
            m_RightPressed = true;
            actions.emplace_back(action);
          } else {
            m_RightPressed = mouseState == GLFW_PRESS;
          }
        }
      }
        break;
    }
  }

  return actions;
}
