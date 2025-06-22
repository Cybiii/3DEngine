#include "Core/Engine.h"
#include "Renderer/Renderer.h"
#include <iostream>

using namespace Engine;

int main() {
  std::cout << "🔺 Basic Triangle Demo" << std::endl;
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
  std::cout << "🎮 Rendering basic triangle..." << std::endl;

  // Main loop
  while (Engine::Engine::IsRunning()) {
    // Clear screen with dark blue background
    Renderer::Clear(0.1f, 0.1f, 0.2f, 1.0f);

    // Draw basic triangle
    Renderer::DrawTriangle();

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