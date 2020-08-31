#include "Window.h"

#include <glad/glad.h>
#include <sstream>

#include "Log.h"
#include "events/Events.h"
#include "EventManager.h"

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            [[maybe_unused]] GLsizei length,
                            const char *message,
                            [[maybe_unused]] const void *userParam);

Chipbit::Window::Window() {
  glfwSetErrorCallback(Window::ErrorCallback);

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  m_Window = glfwCreateWindow(720, 360, "Chipbit", nullptr, nullptr);

  if (!m_Window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(m_Window);
  glfwSetFramebufferSizeCallback(m_Window, Window::ResizeFramebufferCallback);
  glfwSwapInterval(1);
  glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GLFW_TRUE);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    CB_ERROR("Failed to initialize GLAD");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

  if(flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    CB_INFO("Initialized OpenGL Debugging");
  }

  Update();
}

Chipbit::Window::~Window() {
  glfwDestroyWindow(m_Window);
}

bool Chipbit::Window::ShouldClose() {
  return glfwWindowShouldClose(m_Window) != 0;
}

void Chipbit::Window::BeginFrame() {
  Update();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Chipbit::Window::EndFrame() {
  glfwSwapBuffers(m_Window);
  glfwPollEvents();
  m_FrameCount++;
}

void Chipbit::Window::Update() {
  int screenWidth, screenHeight, frameWidth, frameHeight;
  float xScale, yScale;
  double mouseX, mouseY;

  glfwGetWindowSize(m_Window, &screenWidth, &screenHeight);
  glfwGetFramebufferSize(m_Window, &frameWidth, &frameHeight);
  glfwGetWindowContentScale(m_Window, &xScale, &yScale);
  glfwGetCursorPos(m_Window, &mouseX, &mouseY);

  if(m_ScreenSize.x != (float)screenWidth || m_ScreenSize.y != (float)screenHeight) {
    EventManager::Dispatcher().trigger<Events::WindowResizedEvent>(screenWidth, screenHeight);
  }

  m_Scale = glm::vec2(xScale, yScale);
  m_ScreenSize = glm::vec2(screenWidth, screenHeight);
  m_FrameSize = glm::vec2(frameWidth, frameHeight);
  m_MousePos = glm::vec2(mouseX, mouseY);

  glViewport(0, 0, frameWidth, frameHeight);
}

void Chipbit::Window::ErrorCallback(int error, const char *description) {
  CB_ERROR("Error ({0}): {1}", error, description);
}

void Chipbit::Window::ResizeFramebufferCallback([[maybe_unused]] GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            [[maybe_unused]] GLsizei length,
                            const char *message,
                            [[maybe_unused]] const void *userParam)
{
  // ignore non-significant error/warning codes
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204 || id == 131184) return;

  std::stringstream msg;

  msg << '\n';
  msg << "Debug message (" << id << "): " << message << '\n';

  switch (source)
  {
    case GL_DEBUG_SOURCE_API:             msg << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   msg << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: msg << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     msg << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     msg << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           msg << "Source: Other"; break;

    default:
      break;
  }
  msg << '\n';

  switch (type)
  {
    case GL_DEBUG_TYPE_ERROR:               msg << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: msg << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  msg << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         msg << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         msg<< "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              msg << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          msg << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           msg << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               msg << "Type: Other"; break;

    default:
      break;
  }
  msg << '\n';

  switch (severity)
  {
    case GL_DEBUG_SEVERITY_HIGH:         msg << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       msg << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          msg << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: msg << "Severity: notification"; break;

    default:
      break;
  } msg << '\n';
  msg << '\n';

  switch(severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      CB_ERROR(msg.str());
      break;

    case GL_DEBUG_SEVERITY_MEDIUM:
      CB_WARN(msg.str());
      break;

    case GL_DEBUG_SEVERITY_LOW:
      CB_INFO(msg.str());
      break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
      CB_TRACE(msg.str());
      break;

    default:
      break;
  }
}
