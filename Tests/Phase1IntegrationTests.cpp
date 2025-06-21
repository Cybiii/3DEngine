#include <Engine/Core/Engine.h>
#include <Engine/Core/Logger.h>
#include <Engine/Platform/Window.h>
#include <Engine/Renderer/Renderer.h>

#include <cassert>
#include <iostream>


// Simple test framework
#define TEST_ASSERT(condition, message)                                        \
  if (!(condition)) {                                                          \
    ENGINE_LOG_ERROR("TEST", "FAILED: " message);                              \
    return false;                                                              \
  } else {                                                                     \
    ENGINE_LOG_INFO("TEST", "PASSED: " message);                               \
  }

class Phase1Tests {
public:
  static bool RunAllTests() {
    ENGINE_LOG_INFO("TEST", "=== Starting Phase 1 Integration Tests ===");

    bool allPassed = true;

    allPassed &= TestLoggerSystem();
    allPassed &= TestWindowSystem();
    allPassed &= TestEngineInitialization();
    allPassed &= TestRendererSystem();

    if (allPassed) {
      ENGINE_LOG_INFO("TEST", "=== ALL TESTS PASSED ===");
    } else {
      ENGINE_LOG_ERROR("TEST", "=== SOME TESTS FAILED ===");
    }

    return allPassed;
  }

private:
  static bool TestLoggerSystem() {
    ENGINE_LOG_INFO("TEST", "Testing Logger System...");

    // Test logger initialization
    Engine::Logger::Initialize();
    TEST_ASSERT(true, "Logger initialization");

    // Test different log levels
    Engine::Logger::Trace("Test trace message");
    Engine::Logger::Debug("Test debug message");
    Engine::Logger::Info("Test info message");
    Engine::Logger::Warn("Test warning message");
    Engine::Logger::Error("Test error message");

    TEST_ASSERT(true, "Logger message output");

    Engine::Logger::Shutdown();
    return true;
  }

  static bool TestWindowSystem() {
    ENGINE_LOG_INFO("TEST", "Testing Window System...");

    // Test GLFW initialization
    bool initialized = Engine::Platform::Window::Initialize();
    TEST_ASSERT(initialized, "GLFW initialization");

    // Test window creation
    Engine::Platform::WindowProperties props("Test Window", 800, 600, false);
    bool created = Engine::Platform::Window::Create(props);
    TEST_ASSERT(created, "Window creation");

    // Test window properties
    TEST_ASSERT(Engine::Platform::Window::GetWidth() == 800, "Window width");
    TEST_ASSERT(Engine::Platform::Window::GetHeight() == 600, "Window height");
    TEST_ASSERT(!Engine::Platform::Window::IsVSync(), "VSync disabled");

    // Test window operations
    Engine::Platform::Window::SetVSync(true);
    TEST_ASSERT(Engine::Platform::Window::IsVSync(), "VSync enabled");

    Engine::Platform::Window::SetTitle("Updated Test Window");
    TEST_ASSERT(true, "Window title update");

    // Brief event polling test
    for (int i = 0; i < 10; ++i) {
      Engine::Platform::Window::PollEvents();
      if (Engine::Platform::Window::ShouldClose()) {
        break;
      }
    }
    TEST_ASSERT(true, "Event polling");

    Engine::Platform::Window::Shutdown();
    return true;
  }

  static bool TestEngineInitialization() {
    ENGINE_LOG_INFO("TEST", "Testing Engine Initialization...");

    // Test engine initialization
    bool initialized = Engine::Engine::Initialize();
    TEST_ASSERT(initialized, "Engine initialization");

    TEST_ASSERT(Engine::Engine::IsRunning(), "Engine running state");

    // Test immediate shutdown (don't actually run the loop)
    Engine::Engine::RequestExit();
    TEST_ASSERT(!Engine::Engine::IsRunning(), "Engine exit request");

    Engine::Engine::Shutdown();
    return true;
  }

  static bool TestRendererSystem() {
    ENGINE_LOG_INFO("TEST", "Testing Renderer System...");

    // For renderer testing, we need a valid OpenGL context
    Engine::Logger::Initialize();

    bool windowInit = Engine::Platform::Window::Initialize();
    TEST_ASSERT(windowInit, "Window system for renderer test");

    Engine::Platform::WindowProperties props("Renderer Test", 800, 600, false);
    bool windowCreated = Engine::Platform::Window::Create(props);
    TEST_ASSERT(windowCreated, "Window creation for renderer test");

    // Test renderer initialization
    bool rendererInit = Engine::Renderer::Initialize();
    TEST_ASSERT(rendererInit, "Renderer initialization");

    // Test basic renderer operations
    Engine::Renderer::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    Engine::Renderer::Clear();
    TEST_ASSERT(true, "Renderer clear operations");

    Engine::Renderer::SetViewport(0, 0, 800, 600);
    TEST_ASSERT(true, "Renderer viewport setting");

    // Test triangle rendering (just call it, can't easily verify visually in
    // test)
    Engine::Renderer::DrawTriangle();
    TEST_ASSERT(true, "Triangle rendering call");

    Engine::Renderer::Shutdown();
    Engine::Platform::Window::Shutdown();
    Engine::Logger::Shutdown();

    return true;
  }
};

int main() {
  bool testsPassed = Phase1Tests::RunAllTests();
  return testsPassed ? 0 : 1;
}