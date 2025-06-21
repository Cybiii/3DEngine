#include "Core/Engine.h"
#include "Core/Logger.h"

int main() {
  // Initialize engine
  if (!Engine::Engine::Initialize()) {
    ENGINE_LOG_ERROR("Application", "Failed to initialize engine!");
    return -1;
  }

  ENGINE_LOG_INFO("Application", "Starting Triangle Demo...");

  // Run the main loop
  Engine::Engine::Run();

  // Shutdown
  Engine::Engine::Shutdown();

  ENGINE_LOG_INFO("Application", "Triangle Demo ended successfully");
  return 0;
}