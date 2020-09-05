#pragma once

#include <utility>
#include <vector>

namespace Chipbit::Events {
  struct WindowResizedEvent {
    int width;
    int height;
  };

  struct QuitEvent {};

  struct PauseEvent {
    bool paused;
  };

  struct LoadRomEvent {
    std::string rom;
  };

  struct UpdateTPFEvent {
    unsigned int ticksPerFrame;
  };

  struct ResetEvent {};
}
