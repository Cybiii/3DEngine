#include "Engine.h"
#include "../Renderer/Renderer.h"
#include "Logger.h"


#include <GLFW/glfw3.h>

namespace Engine {

bool Engine::s_Running = false;
float Engine::s_LastFrameTime = 0.0f;

bool Engine::Initialize() {
  ENGINE_LOG_INFO("Engine", "Initializing 3D Engine...");

  // Initialize Logger
  Logger::Initialize();

  // Initialize platform layer
  if (!Platform::Window::Initialize()) {
    ENGINE_LOG_ERROR("Engine", "Failed to initialize Window system!");
    return false;
  }

  // Create window
  Platform::WindowProperties props("3D Engine - Phase 1", 1280, 720, true);
  if (!Platform::Window::Create(props)) {
    ENGINE_LOG_ERROR("Engine", "Failed to create window!");
    return false;
  }

  // Set event callback
  Platform::Window::SetEventCallback([](Platform::Event &e) { OnEvent(e); });

  // Initialize renderer
  if (!Renderer::Initialize()) {
    ENGINE_LOG_ERROR("Engine", "Failed to initialize Renderer!");
    return false;
  }

  s_Running = true;
  ENGINE_LOG_INFO("Engine", "Engine initialized successfully!");
  return true;
}

void Engine::Shutdown() {
  ENGINE_LOG_INFO("Engine", "Shutting down engine...");

  Renderer::Shutdown();
  Platform::Window::Shutdown();
  Logger::Shutdown();

  s_Running = false;
  ENGINE_LOG_INFO("Engine", "Engine shutdown complete.");
}

void Engine::Run() {
  ENGINE_LOG_INFO("Engine", "Starting main loop...");

  while (s_Running) {
    float time = static_cast<float>(glfwGetTime());
    float deltaTime = time - s_LastFrameTime;
    s_LastFrameTime = time;

    Platform::Window::PollEvents();

    if (Platform::Window::ShouldClose()) {
      RequestExit();
      continue;
    }

    Update(deltaTime);
    Render();

    Platform::Window::SwapBuffers();
  }

  ENGINE_LOG_INFO("Engine", "Main loop ended.");
}

void Engine::OnEvent(Platform::Event &event) {
  switch (event.Type) {
  case Platform::EventType::WindowClose:
    event.Handled = OnWindowClose(event);
    break;
  case Platform::EventType::WindowResize:
    event.Handled =
        OnWindowResize(static_cast<Platform::WindowResizeEvent &>(event));
    break;
  case Platform::EventType::KeyPressed:
    event.Handled =
        OnKeyPressed(static_cast<Platform::KeyPressedEvent &>(event));
    break;
  }
}

void Engine::Update(float deltaTime) {
  // Update game logic here
  // For Phase 1, we don't have much to update yet
}

void Engine::Render() {
  Renderer::Clear();

  // Render our first triangle!
  Renderer::DrawTriangle();

  // Future: Render scene objects, UI, etc.
}

bool Engine::OnWindowClose(Platform::Event &event) {
  ENGINE_LOG_INFO("Engine", "Window close requested");
  RequestExit();
  return true;
}

bool Engine::OnWindowResize(Platform::WindowResizeEvent &event) {
  ENGINE_LOG_INFO("Engine", "Window resized to " + std::to_string(event.Width) +
                                "x" + std::to_string(event.Height));

  Renderer::SetViewport(0, 0, event.Width, event.Height);
  return true;
}

bool Engine::OnKeyPressed(Platform::KeyPressedEvent &event) {
  // ESC key to exit
  if (event.KeyCode == 256) { // GLFW_KEY_ESCAPE
    ENGINE_LOG_INFO("Engine", "Escape key pressed, exiting...");
    RequestExit();
    return true;
  }

  return false;
}

} // namespace Engine