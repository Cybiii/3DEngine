#include "Window.h"
#include "../Core/Logger.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine {
namespace Platform {

GLFWwindow *Window::s_Window = nullptr;
unsigned int Window::s_Width = 0;
unsigned int Window::s_Height = 0;
bool Window::s_VSync = true;
Window::EventCallbackFn Window::s_EventCallback;

bool Window::Initialize() {
  ENGINE_LOG_INFO("Window", "Initializing GLFW...");

  if (!glfwInit()) {
    ENGINE_LOG_ERROR("Window", "Failed to initialize GLFW!");
    return false;
  }

  glfwSetErrorCallback(GLFWErrorCallback);

  // Set OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  ENGINE_LOG_INFO("Window", "GLFW initialized successfully");
  return true;
}

void Window::Shutdown() {
  ENGINE_LOG_INFO("Window", "Shutting down GLFW...");

  Destroy();
  glfwTerminate();

  ENGINE_LOG_INFO("Window", "GLFW shutdown complete");
}

bool Window::Create(const WindowProperties &props) {
  s_Width = props.Width;
  s_Height = props.Height;

  ENGINE_LOG_INFO("Window", "Creating window: " + props.Title + " (" +
                                std::to_string(props.Width) + "x" +
                                std::to_string(props.Height) + ")");

  s_Window = glfwCreateWindow(static_cast<int>(props.Width),
                              static_cast<int>(props.Height),
                              props.Title.c_str(), nullptr, nullptr);

  if (!s_Window) {
    ENGINE_LOG_ERROR("Window", "Failed to create GLFW window!");
    return false;
  }

  glfwMakeContextCurrent(s_Window);

  // Initialize GLAD
  if (!gladLoadGL()) {
    ENGINE_LOG_ERROR("Window", "Failed to initialize GLAD!");
    return false;
  }

  ENGINE_LOG_INFO("Window", "OpenGL Info:");
  ENGINE_LOG_INFO("Window",
                  "  Vendor: " + std::string(reinterpret_cast<const char *>(
                                     glGetString(GL_VENDOR))));
  ENGINE_LOG_INFO("Window",
                  "  Renderer: " + std::string(reinterpret_cast<const char *>(
                                       glGetString(GL_RENDERER))));
  ENGINE_LOG_INFO("Window",
                  "  Version: " + std::string(reinterpret_cast<const char *>(
                                      glGetString(GL_VERSION))));

  // Set callbacks
  glfwSetWindowSizeCallback(s_Window, GLFWWindowSizeCallback);
  glfwSetKeyCallback(s_Window, GLFWKeyCallback);
  glfwSetMouseButtonCallback(s_Window, GLFWMouseButtonCallback);
  glfwSetCursorPosCallback(s_Window, GLFWCursorPosCallback);
  glfwSetScrollCallback(s_Window, GLFWScrollCallback);
  glfwSetWindowCloseCallback(s_Window, GLFWWindowCloseCallback);

  SetVSync(props.VSync);

  ENGINE_LOG_INFO("Window", "Window created successfully");
  return true;
}

void Window::Destroy() {
  if (s_Window) {
    glfwDestroyWindow(s_Window);
    s_Window = nullptr;
  }
}

void Window::PollEvents() { glfwPollEvents(); }

void Window::SwapBuffers() { glfwSwapBuffers(s_Window); }

bool Window::ShouldClose() {
  return s_Window ? glfwWindowShouldClose(s_Window) : false;
}

void Window::SetVSync(bool enabled) {
  s_VSync = enabled;
  glfwSwapInterval(enabled ? 1 : 0);
}

void Window::SetTitle(const std::string &title) {
  if (s_Window) {
    glfwSetWindowTitle(s_Window, title.c_str());
  }
}

// GLFW Callbacks
void Window::GLFWErrorCallback(int error, const char *description) {
  ENGINE_LOG_ERROR("GLFW",
                   "Error " + std::to_string(error) + ": " + description);
}

void Window::GLFWWindowSizeCallback(GLFWwindow *window, int width, int height) {
  s_Width = width;
  s_Height = height;

  WindowResizeEvent event(width, height);
  if (s_EventCallback) {
    s_EventCallback(event);
  }

  glViewport(0, 0, width, height);
}

void Window::GLFWKeyCallback(GLFWwindow *window, int key, int scancode,
                             int action, int mods) {
  switch (action) {
  case GLFW_PRESS: {
    KeyPressedEvent event(key, 0);
    if (s_EventCallback)
      s_EventCallback(event);
    break;
  }
  case GLFW_RELEASE: {
    KeyReleasedEvent event(key);
    if (s_EventCallback)
      s_EventCallback(event);
    break;
  }
  case GLFW_REPEAT: {
    KeyPressedEvent event(key, 1);
    if (s_EventCallback)
      s_EventCallback(event);
    break;
  }
  }
}

void Window::GLFWMouseButtonCallback(GLFWwindow *window, int button, int action,
                                     int mods) {
  // TODO: Implement mouse button events
}

void Window::GLFWCursorPosCallback(GLFWwindow *window, double xpos,
                                   double ypos) {
  // TODO: Implement mouse move events
}

void Window::GLFWScrollCallback(GLFWwindow *window, double xoffset,
                                double yoffset) {
  // TODO: Implement mouse scroll events
}

void Window::GLFWWindowCloseCallback(GLFWwindow *window) {
  Event event;
  event.Type = EventType::WindowClose;
  if (s_EventCallback)
    s_EventCallback(event);
}

} // namespace Platform
} // namespace Engine