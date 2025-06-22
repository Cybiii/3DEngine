#include "Core/Engine.h"
#include "Renderer/Renderer.h"
#include <iostream>

using namespace Engine;

int main() {
  std::cout << "🔺 Animated Triangle Demo" << std::endl;
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
  std::cout << "🎮 Starting animated triangle demo..." << std::endl;

  // Main loop
  float time = 0.0f;
  while (Engine::Engine::IsRunning()) {
    // Update time
    time += Engine::Engine::GetDeltaTime();

    // Clear screen with dark background
    Renderer::Clear(0.05f, 0.05f, 0.1f, 1.0f);

    // Draw animated triangle
    Renderer::DrawAnimatedTriangle(time);

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