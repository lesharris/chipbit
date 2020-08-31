#include "Input.h"

#include "../Chipbit.h"

Chipbit::Input::Input() {
  addMapping(GLFW_KEY_7, Action::K1, ActionSource::Keyboard);
  addMapping(GLFW_KEY_8, Action::K2, ActionSource::Keyboard);
  addMapping(GLFW_KEY_9, Action::K3, ActionSource::Keyboard);
  addMapping(GLFW_KEY_0, Action::KC, ActionSource::Keyboard);
  addMapping(GLFW_KEY_U, Action::K4, ActionSource::Keyboard);
  addMapping(GLFW_KEY_I, Action::K5, ActionSource::Keyboard);
  addMapping(GLFW_KEY_O, Action::K6, ActionSource::Keyboard);
  addMapping(GLFW_KEY_P, Action::KD, ActionSource::Keyboard);
  addMapping(GLFW_KEY_J, Action::K7, ActionSource::Keyboard);
  addMapping(GLFW_KEY_K, Action::K8, ActionSource::Keyboard);
  addMapping(GLFW_KEY_L, Action::K9, ActionSource::Keyboard);
  addMapping(GLFW_KEY_SEMICOLON, Action::KE, ActionSource::Keyboard);
  addMapping(GLFW_KEY_M, Action::KA, ActionSource::Keyboard);
  addMapping(GLFW_KEY_COMMA, Action::K0, ActionSource::Keyboard);
  addMapping(GLFW_KEY_PERIOD, Action::KB, ActionSource::Keyboard);
  addMapping(GLFW_KEY_SLASH, Action::KF, ActionSource::Keyboard);
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
