#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>


#ifdef _WIN32
#include <Windows.h>
#endif

namespace Engine {

LogLevel Logger::s_LogLevel = LogLevel::Trace;

void Logger::Initialize() {
#ifdef _WIN32
  // Enable ANSI color codes on Windows
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
#endif

  Info("Logger", "Logger initialized");
}

void Logger::Shutdown() { Info("Logger", "Logger shutdown"); }

void Logger::Log(LogLevel level, const std::string &message) {
  Log(level, "Engine", message);
}

void Logger::Log(LogLevel level, const std::string &tag,
                 const std::string &message) {
  if (level < s_LogLevel)
    return;

  // Get current time
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;

  std::ostringstream oss;
  oss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
  oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

  // Format: [HH:MM:SS.mmm] [LEVEL] [TAG] Message
  std::cout << LogLevelToColor(level) << "[" << oss.str() << "] "
            << "[" << LogLevelToString(level) << "] "
            << "[" << tag << "] " << message << "\033[0m" // Reset color
            << std::endl;
}

const char *Logger::LogLevelToString(LogLevel level) {
  switch (level) {
  case LogLevel::Trace:
    return "TRACE";
  case LogLevel::Debug:
    return "DEBUG";
  case LogLevel::Info:
    return "INFO ";
  case LogLevel::Warn:
    return "WARN ";
  case LogLevel::Error:
    return "ERROR";
  case LogLevel::Critical:
    return "CRIT ";
  default:
    return "UNKN ";
  }
}

const char *Logger::LogLevelToColor(LogLevel level) {
  switch (level) {
  case LogLevel::Trace:
    return "\033[37m"; // White
  case LogLevel::Debug:
    return "\033[36m"; // Cyan
  case LogLevel::Info:
    return "\033[32m"; // Green
  case LogLevel::Warn:
    return "\033[33m"; // Yellow
  case LogLevel::Error:
    return "\033[31m"; // Red
  case LogLevel::Critical:
    return "\033[35m"; // Magenta
  default:
    return "\033[37m"; // White
  }
}

} // namespace Engine