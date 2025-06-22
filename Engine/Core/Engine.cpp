#include "Engine.h"
#include "../Math/Math.h"
#include "../Renderer/Renderer.h"
#include "Camera.h"
#include "Logger.h"

#include <GLFW/glfw3.h>

namespace Engine {

bool Engine::s_Running = false;
float Engine::s_LastFrameTime = 0.0f;
float Engine::s_DeltaTime = 0.0f;

bool Engine::Initialize() {
  Logger::Info("Engine", "Initializing 3D Engine...");

  // Initialize Logger
  Logger::Initialize();

  // Initialize platform layer
  if (!Platform::Window::Initialize()) {
    Logger::Error("Engine", "Failed to initialize Window system!");
    return false;
  }

  // Create window
  Platform::WindowProperties props("3D Engine - Phase 2", 1280, 720, true);
  if (!Platform::Window::Create(props)) {
    Logger::Error("Engine", "Failed to create window!");
    return false;
  }

  // Set event callback
  Platform::Window::SetEventCallback([](Platform::Event &e) { OnEvent(e); });

  // Initialize renderer
  if (!Renderer::Initialize()) {
    Logger::Error("Engine", "Failed to initialize Renderer!");
    return false;
  }

  s_Running = true;
  Logger::Info("Engine", "Engine initialized successfully!");
  return true;
}

void Engine::Shutdown() {
  Logger::Info("Engine", "Shutting down engine...");

  Renderer::Shutdown();
  Platform::Window::Shutdown();
  Logger::Shutdown();

  s_Running = false;
  Logger::Info("Engine", "Engine shutdown complete.");
}

// Manual loop support for demos
void Engine::Update() {
  float time = static_cast<float>(glfwGetTime());
  s_DeltaTime = time - s_LastFrameTime;
  s_LastFrameTime = time;

  Platform::Window::PollEvents();

  if (Platform::Window::ShouldClose()) {
    RequestExit();
  }

  Platform::Window::SwapBuffers();
}

float Engine::GetDeltaTime() { return s_DeltaTime; }

Platform::Window *Engine::GetWindow() {
  return nullptr; // TODO: Implement proper window wrapper
}

void Engine::Run() {
  Logger::Info("Engine", "Starting main loop...");

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

  Logger::Info("Engine", "Main loop ended.");
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
  Renderer::Clear(0.1f, 0.1f, 0.2f, 1.0f);

  // Phase 2: Render a simple rotating cube
  static float time = 0.0f;
  time += 0.016f; // Approximate 60 FPS

  // Create camera
  Camera camera;
  camera.SetPosition(Vec3(0.0f, 0.0f, 5.0f));
  camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));
  camera.SetFieldOfView(45.0f);
  camera.SetAspectRatio(1280.0f, 720.0f);

  // Create rotating transform
  Transform cubeTransform;
  cubeTransform.position = Vec3(0.0f, 0.0f, 0.0f);
  cubeTransform.scale = Vec3(1.0f, 1.0f, 1.0f);

  // Rotate around Y and X axes
  float rotationY = time * 45.0f; // degrees
  float rotationX = time * 20.0f; // degrees
  Quaternion yRot =
      Quaternion::FromAxisAngle(Vec3::Up(), Math::ToRadians(rotationY));
  Quaternion xRot =
      Quaternion::FromAxisAngle(Vec3::Right(), Math::ToRadians(rotationX));
  cubeTransform.rotation = yRot * xRot;

  // Render solid cube
  Renderer::DrawCube(camera, cubeTransform, Vec3(0.8f, 0.6f, 0.4f));

  // Render wireframe cube slightly larger
  Transform wireTransform = cubeTransform;
  wireTransform.scale = Vec3(1.1f, 1.1f, 1.1f);
  Renderer::DrawWireCube(camera, wireTransform, Vec3(1.0f, 1.0f, 1.0f));
}

bool Engine::OnWindowClose(Platform::Event &event) {
  Logger::Info("Engine", "Window close requested");
  RequestExit();
  return true;
}

bool Engine::OnWindowResize(Platform::WindowResizeEvent &event) {
  Logger::Info("Engine", "Window resized to " + std::to_string(event.Width) +
                             "x" + std::to_string(event.Height));

  Renderer::SetViewport(0, 0, event.Width, event.Height);
  return true;
}

bool Engine::OnKeyPressed(Platform::KeyPressedEvent &event) {
  // ESC key to exit
  if (event.KeyCode == 256) { // GLFW_KEY_ESCAPE
    Logger::Info("Engine", "Escape key pressed, exiting...");
    RequestExit();
    return true;
  }

  return false;
}

} // namespace Engine