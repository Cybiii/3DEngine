#include "Core/Camera.h"
#include "Core/Engine.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include <iostream>

using namespace Engine;

int main() {
  std::cout << "🎲 Basic Cube Demo" << std::endl;
  std::cout << "Press ESC to exit" << std::endl;

  // Initialize the engine
  if (!Engine::Engine::Initialize()) {
    std::cerr << "❌ Failed to initialize engine!" << std::endl;
    return -1;
  }

  // Initialize renderer
  if (!Renderer::Initialize()) {
    std::cerr << "❌ Failed to initialize renderer!" << std::endl;
    Engine::Engine::Shutdown();
    return -1;
  }

  std::cout << "✅ Engine and Renderer initialized successfully!" << std::endl;
  std::cout << "🎮 Rendering basic cube..." << std::endl;

  // Create camera
  Camera camera;
  camera.SetPosition(Vec3(0.0f, 0.0f, 5.0f));
  camera.LookAt(Vec3(0.0f, 0.0f, 0.0f));
  camera.SetAspectRatio(800.0f, 600.0f);
  camera.SetFieldOfView(45.0f);

  // Create transform
  Transform transform;
  transform.position = Vec3(0.0f, 0.0f, 0.0f);
  transform.rotation = Quaternion::Identity();
  transform.scale = Vec3(1.0f, 1.0f, 1.0f);

  // Main loop
  while (Engine::Engine::IsRunning()) {
    // Clear screen with dark background
    Renderer::Clear(0.1f, 0.1f, 0.2f, 1.0f);

    // Draw basic cube
    Vec3 cubeColor = Vec3(0.8f, 0.4f, 0.2f); // Orange color
    Renderer::DrawCube(camera, transform, cubeColor);

    // Update engine (handles events and buffer swapping)
    Engine::Engine::Update();
  }

  std::cout << "🛑 Shutting down..." << std::endl;

  // Cleanup
  Renderer::Shutdown();
  Engine::Engine::Shutdown();

  std::cout << "✅ Cleanup complete. Goodbye!" << std::endl;
  return 0;
}