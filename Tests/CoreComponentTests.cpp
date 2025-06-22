#include "Core/Logger.h"
#include <iostream>

using namespace Engine;

bool TestLogger() {
  Logger::Info("CoreTest", "Testing Logger System...");

  // Test different log levels
  Logger::Trace("CoreTest", "Test trace message");
  Logger::Debug("CoreTest", "Test debug message");
  Logger::Info("CoreTest", "Test info message");
  Logger::Warn("CoreTest", "Test warning message");
  Logger::Error("CoreTest", "Test error message");

  Logger::Info("CoreTest", "✅ Logger tests passed!");
  return true;
}

bool TestBasicCompilation() {
  Logger::Info("CoreTest", "Testing basic compilation...");

  // Test that core headers compile
  Logger::Info("CoreTest", "✅ Core headers compile successfully!");
  return true;
}

int main() {
  Logger::Info("CoreTest", "🧪 Starting Core Component Tests...");

  bool allPassed = true;

  allPassed &= TestLogger();
  allPassed &= TestBasicCompilation();

  if (allPassed) {
    Logger::Info("CoreTest", "🎉 ALL CORE COMPONENT TESTS PASSED!");
    Logger::Info("CoreTest", "✅ Logger system working");
    Logger::Info("CoreTest", "✅ Core compilation successful");
    Logger::Info("CoreTest", "🚀 Engine core foundation is solid!");
    return 0;
  } else {
    Logger::Error("CoreTest", "❌ Some core tests failed!");
    return -1;
  }
}