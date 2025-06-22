# 3D Engine

A modern, modular 3D graphics engine built from scratch in C++ using OpenGL. This project demonstrates advanced graphics programming concepts and serves as a foundation for 3D applications and games.

## Project Overview

This 3D engine is being developed in phases, starting with core foundational systems and gradually adding advanced rendering features. The current implementation showcases professional software architecture with clean abstractions and modern C++ practices.



##  Architecture

```
3DEngine/
├── Engine/                 # Core engine library
│   ├── Core/              # Engine core systems
│   │   ├── Engine.h/cpp   # Main engine class
│   │   └── Logger.h/cpp   # Logging system
│   ├── Platform/          # Platform abstraction
│   │   └── Window.h/cpp   # Window management (GLFW)
│   └── Renderer/          # Rendering systems
│       ├── Renderer.h/cpp # Main renderer
│       ├── Shader.h/cpp   # Shader compilation
│       ├── Buffer.h/cpp   # Vertex/Index buffers
│       └── VertexArray.h/cpp # VAO management
├── Examples/              # Demo applications
│   └── TriangleDemo.cpp   # Basic triangle rendering
├── ThirdParty/           # External dependencies
│   └── glad/             # OpenGL function loader
├── Shaders/              # GLSL shader files
└── Tests/                # Integration tests
```

## Dependencies

- **OpenGL 4.5+**: Modern graphics API
- **GLFW 3.3+**: Window management and input
- **GLAD**: OpenGL function loader
- **GLM**: Mathematics library
- **C++17**: Modern C++ standard

## Quick Start

### Prerequisites

- **Windows**: MinGW-w64 or Visual Studio 2019+
- **Linux**: GCC 8+ or Clang 10+
- **macOS**: Xcode 12+ or Clang 10+

### Manual Build (Recommended for testing)

```bash
# Clone the repository
git clone
cd 3DEngine

# Run the test build script (Windows)
./test_build.bat

# The executable will be created in test_build/TriangleDemo.exe
cd test_build
./TriangleDemo.exe
```

### CMake Build (Cross-platform)

```bash
# Create build directory
mkdir build && cd build

# Configure (adjust generator as needed)
cmake -G "MinGW Makefiles" ..
# or
cmake -G "Visual Studio 17 2022" ..

# Build
cmake --build .

# Run the demo
./Examples/TriangleDemo
```

**Controls:**

- `ESC` or close window to exit
- Window is resizable with automatic viewport adjustment

## 🔧 Technical Details

### Graphics Pipeline

- **OpenGL 4.5 Core Profile**: Modern, forward-compatible rendering
- **Vertex Specification**: Flexible vertex attribute layout system
- **Shader Management**: Automatic compilation with error reporting
- **Resource Binding**: RAII-based OpenGL object management

### Engine Systems

- **Initialization**: Proper startup/shutdown sequencing
- **Main Loop**: Frame-based rendering with delta time
- **Event Handling**: Callback-based window and input events
- **Memory Management**: Smart pointers and automatic cleanup

## Testing

Integration tests are included to verify engine functionality:

```bash
# Build and run tests
cd build
ctest --verbose

# Or run manually
./Tests/Phase1IntegrationTests
```

## Logging

The engine features a comprehensive logging system:

```cpp
ENGINE_LOG_TRACE("System", "Detailed debug information");
ENGINE_LOG_DEBUG("System", "Debug messages");
ENGINE_LOG_INFO("System", "General information");
ENGINE_LOG_WARN("System", "Warning messages");
ENGINE_LOG_ERROR("System", "Error conditions");
ENGINE_LOG_CRITICAL("System", "Critical failures");
```

Output includes:

- Colored console output (if supported)
- Timestamps with millisecond precision
- System/component identification
- Structured message formatting

## Troubleshooting

### Common Issues

**"Failed to initialize GLFW"**

- Ensure graphics drivers are up to date
- Check OpenGL 4.5 support on your hardware

**"Shader compilation failure"**

- Verify shader source files are present
- Check OpenGL context creation

**"GLAD initialization failed"**

- Ensure OpenGL context is current before calling gladLoadGL()
- Check graphics driver compatibility
