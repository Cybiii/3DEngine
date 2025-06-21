# 3D Engine

A modern, modular 3D graphics engine built from scratch in C++ using OpenGL. This project demonstrates advanced graphics programming concepts and serves as a foundation for 3D applications and games.

## 🎯 Project Overview

This 3D engine is being developed in phases, starting with core foundational systems and gradually adding advanced rendering features. The current implementation showcases professional software architecture with clean abstractions and modern C++ practices.

**Current Status: Phase 1 Complete ✅**

## 🚀 Phase 1 Features

- ✅ **Modular Architecture**: Clean separation between Core, Platform, and Renderer systems
- ✅ **Professional Logging**: Color-coded console output with timestamps and log levels
- ✅ **Cross-Platform Window Management**: GLFW integration with OpenGL 4.5 Core context
- ✅ **Modern OpenGL Rendering**: Shader pipeline with vertex arrays and buffers
- ✅ **Resource Management**: RAII principles with smart pointers
- ✅ **Event System**: Window and input event handling
- ✅ **Working Demo**: Colored triangle rendering with vertex attributes

## 🏗️ Architecture

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

## 🛠️ Dependencies

- **OpenGL 4.5+**: Modern graphics API
- **GLFW 3.3+**: Window management and input
- **GLAD**: OpenGL function loader
- **GLM**: Mathematics library (planned for Phase 2)
- **C++17**: Modern C++ standard

## 🚀 Quick Start

### Prerequisites

- **Windows**: MinGW-w64 or Visual Studio 2019+
- **Linux**: GCC 8+ or Clang 10+
- **macOS**: Xcode 12+ or Clang 10+

### Manual Build (Recommended for testing)

```bash
# Clone the repository
git clone <your-repo-url>
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

## 🎮 Demo

The current demo renders a colorful triangle with:

- **Red vertex** at bottom-left
- **Green vertex** at bottom-right
- **Blue vertex** at top center

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

### Code Quality

- **Modern C++17**: Smart pointers, RAII, type safety
- **Modular Design**: Clear interfaces and dependency injection
- **Error Handling**: Comprehensive logging and validation
- **Cross-Platform**: Abstracted platform-specific code

## 📋 Development Roadmap

### Phase 2: Math Foundation (Next)

- [ ] GLM integration for 3D mathematics
- [ ] Camera system with view/projection matrices
- [ ] 3D transformations and model matrices
- [ ] Input handling system

### Phase 3: Advanced Rendering

- [ ] 3D mesh loading and rendering
- [ ] Texture system and material properties
- [ ] Multiple shader support
- [ ] Basic lighting (Phong/Blinn-Phong)

### Phase 4: Scene Management

- [ ] Scene graph and entity system
- [ ] Frustum culling and spatial optimization
- [ ] Multi-object rendering
- [ ] Transform hierarchies

### Phase 5: Advanced Features

- [ ] Shadow mapping
- [ ] Post-processing pipeline
- [ ] Physically Based Rendering (PBR)
- [ ] Deferred rendering

## 🧪 Testing

Integration tests are included to verify engine functionality:

```bash
# Build and run tests
cd build
ctest --verbose

# Or run manually
./Tests/Phase1IntegrationTests
```

## 📝 Logging

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

## 🐛 Troubleshooting

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

### Build Issues

**CMake configuration fails**

- Try different generators: `-G "MinGW Makefiles"` or `-G "Ninja"`
- Ensure all dependencies are installed
- Clear build directory and retry

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **GLFW Team**: Cross-platform window management and input
- **OpenGL Community**: Graphics API and documentation
- **Modern OpenGL Tutorials**: Learning resources and best practices
- **C++ Community**: Language standards and libraries

## 📞 Contact

- **GitHub**: [Your GitHub Profile]
- **Email**: [Your Email]
- **Discord**: [Your Discord]

---

**Built with ❤️ and modern C++**
