#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>


namespace Engine {

enum class LogLevel {
  Trace = 0,
  Debug = 1,
  Info = 2,
  Warn = 3,
  Error = 4,
  Critical = 5
};

class Logger {
public:
  static void Initialize();
  static void Shutdown();

  static void Log(LogLevel level, const std::string &message);
  static void Log(LogLevel level, const std::string &tag,
                  const std::string &message);

  static void SetLevel(LogLevel level) { s_LogLevel = level; }
  static LogLevel GetLevel() { return s_LogLevel; }

  // Convenience methods
  static void Trace(const std::string &message) {
    Log(LogLevel::Trace, message);
  }
  static void Debug(const std::string &message) {
    Log(LogLevel::Debug, message);
  }
  static void Info(const std::string &message) { Log(LogLevel::Info, message); }
  static void Warn(const std::string &message) { Log(LogLevel::Warn, message); }
  static void Error(const std::string &message) {
    Log(LogLevel::Error, message);
  }
  static void Critical(const std::string &message) {
    Log(LogLevel::Critical, message);
  }

  static void Trace(const std::string &tag, const std::string &message) {
    Log(LogLevel::Trace, tag, message);
  }
  static void Debug(const std::string &tag, const std::string &message) {
    Log(LogLevel::Debug, tag, message);
  }
  static void Info(const std::string &tag, const std::string &message) {
    Log(LogLevel::Info, tag, message);
  }
  static void Warn(const std::string &tag, const std::string &message) {
    Log(LogLevel::Warn, tag, message);
  }
  static void Error(const std::string &tag, const std::string &message) {
    Log(LogLevel::Error, tag, message);
  }
  static void Critical(const std::string &tag, const std::string &message) {
    Log(LogLevel::Critical, tag, message);
  }

private:
  static LogLevel s_LogLevel;
  static const char *LogLevelToString(LogLevel level);
  static const char *LogLevelToColor(LogLevel level);
};

} // namespace Engine

// Macros for easier logging
#ifdef ENGINE_DEBUG
#define ENGINE_LOG_TRACE(...) ::Engine::Logger::Trace(__VA_ARGS__)
#define ENGINE_LOG_DEBUG(...) ::Engine::Logger::Debug(__VA_ARGS__)
#define ENGINE_LOG_INFO(...) ::Engine::Logger::Info(__VA_ARGS__)
#define ENGINE_LOG_WARN(...) ::Engine::Logger::Warn(__VA_ARGS__)
#define ENGINE_LOG_ERROR(...) ::Engine::Logger::Error(__VA_ARGS__)
#define ENGINE_LOG_CRITICAL(...) ::Engine::Logger::Critical(__VA_ARGS__)
#else
#define ENGINE_LOG_TRACE(...)
#define ENGINE_LOG_DEBUG(...)
#define ENGINE_LOG_INFO(...) ::Engine::Logger::Info(__VA_ARGS__)
#define ENGINE_LOG_WARN(...) ::Engine::Logger::Warn(__VA_ARGS__)
#define ENGINE_LOG_ERROR(...) ::Engine::Logger::Error(__VA_ARGS__)
#define ENGINE_LOG_CRITICAL(...) ::Engine::Logger::Critical(__VA_ARGS__)
#endif