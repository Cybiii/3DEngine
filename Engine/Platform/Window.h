#pragma once

#include <functional>
#include <string>


struct GLFWwindow;

namespace Engine {
namespace Platform {

struct WindowProperties {
  std::string Title;
  unsigned int Width;
  unsigned int Height;
  bool VSync;

  WindowProperties(const std::string &title = "3D Engine",
                   unsigned int width = 1280, unsigned int height = 720,
                   bool vsync = true)
      : Title(title), Width(width), Height(height), VSync(vsync) {}
};

// Event system (simplified for Phase 1)
enum class EventType {
  None = 0,
  WindowClose,
  WindowResize,
  KeyPressed,
  KeyReleased,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

struct Event {
  EventType Type = EventType::None;
  bool Handled = false;
};

struct WindowResizeEvent : public Event {
  unsigned int Width, Height;
  WindowResizeEvent(unsigned int width, unsigned int height)
      : Width(width), Height(height) {
    Type = EventType::WindowResize;
  }
};

struct KeyEvent : public Event {
  int KeyCode;
  KeyEvent(int keycode) : KeyCode(keycode) {}
};

struct KeyPressedEvent : public KeyEvent {
  int RepeatCount;
  KeyPressedEvent(int keycode, int repeatCount)
      : KeyEvent(keycode), RepeatCount(repeatCount) {
    Type = EventType::KeyPressed;
  }
};

struct KeyReleasedEvent : public KeyEvent {
  KeyReleasedEvent(int keycode) : KeyEvent(keycode) {
    Type = EventType::KeyReleased;
  }
};

class Window {
public:
  using EventCallbackFn = std::function<void(Event &)>;

  static bool Initialize();
  static void Shutdown();

  static bool Create(const WindowProperties &props = WindowProperties());
  static void Destroy();

  static void PollEvents();
  static void SwapBuffers();
  static bool ShouldClose();

  static void SetEventCallback(const EventCallbackFn &callback) {
    s_EventCallback = callback;
  }
  static void SetVSync(bool enabled);
  static bool IsVSync() { return s_VSync; }

  static unsigned int GetWidth() { return s_Width; }
  static unsigned int GetHeight() { return s_Height; }
  static GLFWwindow *GetNativeWindow() { return s_Window; }

  static void SetTitle(const std::string &title);

private:
  static GLFWwindow *s_Window;
  static unsigned int s_Width, s_Height;
  static bool s_VSync;
  static EventCallbackFn s_EventCallback;

  // GLFW Callbacks
  static void GLFWErrorCallback(int error, const char *description);
  static void GLFWWindowSizeCallback(GLFWwindow *window, int width, int height);
  static void GLFWKeyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mods);
  static void GLFWMouseButtonCallback(GLFWwindow *window, int button,
                                      int action, int mods);
  static void GLFWCursorPosCallback(GLFWwindow *window, double xpos,
                                    double ypos);
  static void GLFWScrollCallback(GLFWwindow *window, double xoffset,
                                 double yoffset);
  static void GLFWWindowCloseCallback(GLFWwindow *window);
};

} // namespace Platform
} // namespace Engine