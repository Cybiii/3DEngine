#include "Core/Camera.h"
#include "Core/Engine.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include <iostream>


using namespace Engine;

int main() {
  std::cout << "🎲 Rotating Cube Demo" << std::endl;
  std::cout << "Press ESC to exit" << std::endl;

  // Initialize the engine
  if (!Engine::Initialize()) {
    std::cerr << "Failed to initialize engine!" << std::endl;
    return -1;
  }

  // Initialize renderer
  if (!Renderer::Initialize()) {
    std::cerr << "Failed to initialize renderer!" << std::endl;
    Engine::Shutdown();
    return -1;
  }

  std::cout << "✅ Engine and Renderer initialized successfully!" << std::endl;
  std::cout << "🎮 Starting rotating cube animation..." << std::endl;

  // Create camera
  Camera camera;
  camera.SetPosition(Vec3(0.0f, 0.0f, 5.0f));
  camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));
  camera.SetAspectRatio(800.0f, 600.0f);
  camera.SetFieldOfView(45.0f);

  // Main loop
  float time = 0.0f;
  while (Engine::IsRunning()) {
    // Update time
    time += Engine::GetDeltaTime();

    // Clear screen
    Renderer::Clear(0.1f, 0.1f, 0.2f, 1.0f); // Dark blue background

    // Create rotating transform
    Transform transform;
    transform.position = Vec3(0.0f, 0.0f, 0.0f);
    transform.rotation = Quaternion::FromAxisAngle(Vec3::Up(), time) *
                         Quaternion::FromAxisAngle(Vec3::Right(), time * 0.5f);
    transform.scale = Vec3(1.0f, 1.0f, 1.0f);

    // Draw rotating cube
    Vec3 cubeColor = Vec3(0.5f + 0.5f * Math::Sin(time),
                          0.5f + 0.5f * Math::Sin(time * 1.3f),
                          0.5f + 0.5f * Math::Sin(time * 0.7f));
    Renderer::DrawCube(camera, transform, cubeColor);

    // Update engine (handles events and buffer swapping)
    Engine::Update();
  }

  std::cout << "🛑 Shutting down..." << std::endl;

  // Cleanup
  Renderer::Shutdown();
  Engine::Shutdown();

  std::cout << "✅ Cleanup complete. Goodbye!" << std::endl;
  return 0;
}