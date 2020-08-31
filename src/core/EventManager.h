#pragma once

#include <entt/entt.hpp>

namespace Chipbit {
  class EventManager {
  public:
    static EventManager& Get() {
      static EventManager instance;

      return instance;
    }

    static entt::dispatcher& Dispatcher() { return Get().m_Dispatcher; }

    template<typename EventStruct, auto Handler, class Listener>
    constexpr void Attach(Listener *const inListener) {
      auto sink = m_Dispatcher.template sink<EventStruct>();
      sink.template connect<Handler>(*inListener);
    }

    template<class Listener, typename... Events>
    constexpr void DetachAll(Listener *const inListener) {
      ((m_Dispatcher.template sink<Events>().
          template disconnect(*inListener)), ...);
    }

  private:
    entt::dispatcher m_Dispatcher{};
  };
}