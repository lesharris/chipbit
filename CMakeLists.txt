cmake_minimum_required(VERSION 3.17)
project(chipbit)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
  set(CMAKE_PREFIX_PATH C:/lib)
  if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")
    message(${CMAKE_CXX_FLAGS})
  endif ()
endif ()

add_subdirectory(deps/spdlog)
add_subdirectory(deps/fmt)

find_package(glfw3 3.3 REQUIRED)

include_directories(deps)
include_directories(deps/imgui)
include_directories(deps/entt)
include_directories(deps/glad/include)
include_directories(deps/spdlog/include)
include_directories(deps/fmt/include)

add_executable(chipbit src/main.cpp
    deps/glad/src/glad.c
    deps/imgui/imgui_demo.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_impl_glfw.cpp
    deps/imgui/imgui_impl_opengl3.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/imgui.cpp
    deps/stb_image.h
    src/Chipbit.cpp
    src/Chipbit.h
    src/core/Window.cpp
    src/core/Window.h
    src/core/Log.h src/core/EventManager.h src/core/events/Events.h src/core/Actions.h src/core/Input.h src/core/Input.cpp src/loaders/ShaderLoader.cpp src/loaders/ShaderLoader.h src/renderer/Shader.cpp src/renderer/Shader.h src/renderer/UniformType.h src/cpu/Chip8.cpp src/cpu/Chip8.h src/loaders/RomLoader.cpp src/loaders/RomLoader.h src/renderer/Renderer.cpp src/renderer/Renderer.h)

target_link_libraries(chipbit PRIVATE
    glfw
    spdlog
    fmt)
