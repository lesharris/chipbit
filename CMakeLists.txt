cmake_minimum_required(VERSION 3.17)
project(chipbit)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
  set(CMAKE_PREFIX_PATH C:/lib)
  include_directories(deps/dirent/include)
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
    deps/imgui_memory_editor.h
    deps/imgui/imgui_demo.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_impl_glfw.cpp
    deps/imgui/imgui_impl_opengl3.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/imgui.cpp
    deps/stb_image.h
    src/Chipbit.cpp
    src/Chipbit.h
    src/core/Actions.h
    src/core/events/Events.h
    src/core/Input.cpp
    src/core/Input.h
    src/core/Log.h src/core/EventManager.h
    src/core/Window.cpp
    src/core/Window.h
    src/cpu/Chip8.cpp
    src/cpu/Chip8.h
    src/gui/Gui.cpp
    src/gui/Gui.h
    src/gui/widgets/Widget.h
    src/loaders/RomLoader.cpp
    src/loaders/RomLoader.h
    src/loaders/ShaderLoader.cpp
    src/loaders/ShaderLoader.h
    src/renderer/Renderer.cpp
    src/renderer/Renderer.h
    src/renderer/Shader.cpp
    src/renderer/Shader.h
    src/renderer/UniformType.h
    src/gui/widgets/RegisterView.cpp
    src/gui/widgets/RegisterView.h
    src/gui/widgets/MemoryView.cpp
    src/gui/widgets/MemoryView.h
    src/gui/widgets/StackView.cpp
    src/gui/widgets/StackView.h
    src/gui/widgets/Widgets.h
    deps/TextEditor.cpp
    deps/TextEditor.h
    src/gui/widgets/CodeEditor.cpp
    src/gui/widgets/CodeEditor.h
    deps/ImGuiFileDialog.cpp src/gui/widgets/Main.cpp src/gui/widgets/Main.h)

target_link_libraries(chipbit PRIVATE
    glfw
    spdlog
    fmt)