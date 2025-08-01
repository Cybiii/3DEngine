#pragma once

#include "../Platform/Window.h"

namespace Engine {

class Engine {
public:
  static bool Initialize();
  static void Shutdown();
  static void Run();

  // Manual loop support for demos
  static void Update();
  static float GetDeltaTime();
  static Platform::Window *GetWindow();

  static bool IsRunning() { return s_Running; }
  static void RequestExit() { s_Running = false; }

  // Event handling
  static void OnEvent(Platform::Event &event);

private:
  static bool s_Running;
  static float s_LastFrameTime;
  static float s_DeltaTime;

  static void Update(float deltaTime);
  static void Render();

  // Event handlers
  static bool OnWindowClose(Platform::Event &event);
  static bool OnWindowResize(Platform::WindowResizeEvent &event);
  static bool OnKeyPressed(Platform::KeyPressedEvent &event);
};

} // namespace Engine