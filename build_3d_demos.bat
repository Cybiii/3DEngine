@echo off
echo Building 3D Demos...

cd test_build

echo.
echo =================================
echo Building CubeDemo...
echo =================================
g++ -std=c++17 -I../Engine -I../ThirdParty/glad/include -I./glfw/include ^
    ../Examples/CubeDemo.cpp ^
    Engine.o Logger.o Window.o Renderer.o Shader.o Buffer.o VertexArray.o MathTypes.o glad.o ^
    -L./glfw/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 ^
    -o CubeDemo.exe

if %ERRORLEVEL% NEQ 0 (
    echo CubeDemo build failed!
    pause
    exit /b 1
)

echo CubeDemo.exe built successfully!

echo.
echo =================================
echo Building DebugCubeDemo...
echo =================================
g++ -std=c++17 -I../Engine -I../ThirdParty/glad/include -I./glfw/include ^
    ../Examples/DebugCubeDemo.cpp ^
    Engine.o Logger.o Window.o Renderer.o Shader.o Buffer.o VertexArray.o MathTypes.o glad.o ^
    -L./glfw/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 ^
    -o DebugCubeDemo.exe

if %ERRORLEVEL% NEQ 0 (
    echo DebugCubeDemo build failed!
    pause
    exit /b 1
)

echo DebugCubeDemo.exe built successfully!

echo.
echo =================================
echo 3D Demos built successfully!
echo =================================
echo Available demos:
echo - CubeDemo.exe (Full 3D demo with interactive camera)
echo - DebugCubeDemo.exe (Step-by-step debugging demo)
echo.

cd .. 