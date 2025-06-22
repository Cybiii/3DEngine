#include "Core/Engine.h"
#include "Renderer/Renderer.h"
#include <iostream>

using namespace Engine;

int main() {
  std::cout << "🔺 Spinning Triangle Demo" << std::endl;
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
  std::cout << "🎮 Starting spinning triangle animation..." << std::endl;

  // Main loop
  float time = 0.0f;
  while (Engine::IsRunning()) {
    // Update time
    time += Engine::GetDeltaTime();

    // Clear screen
    Renderer::Clear(0.1f, 0.1f, 0.2f, 1.0f); // Dark blue background

    // Draw animated triangle
    Renderer::DrawAnimatedTriangle(time);

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