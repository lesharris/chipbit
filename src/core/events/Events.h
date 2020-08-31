#pragma once

#include <utility>
#include <vector>

namespace Chipbit::Events {
  struct WindowResizedEvent {
    int width;
    int height;
  };

  struct ScreenUpdateEvent {
    explicit ScreenUpdateEvent(std::vector<unsigned short> fb) : framebuffer(std::move(fb)) {}

    std::vector<unsigned short> framebuffer;
  };
}
