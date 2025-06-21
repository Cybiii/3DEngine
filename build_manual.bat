@echo off
echo Building 3D Engine manually...

REM Create build directory
if not exist "manual_build" mkdir manual_build
cd manual_build

REM Download GLFW binaries (simpler than building)
echo Downloading GLFW...
if not exist "glfw" (
    curl -L -o glfw.zip "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip"
    if exist glfw.zip (
        powershell -command "Expand-Archive glfw.zip -DestinationPath ."
        ren glfw-3.3.8.bin.WIN64 glfw
        del glfw.zip
    )
)

REM Check if we have a C++ compiler available
where g++ >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: g++ not found in PATH
    echo Please install MinGW-w64 or MSYS2
    pause
    exit /b 1
)

echo Compiling engine...

REM Compile GLAD
g++ -c ../ThirdParty/glad/src/glad.c -I../ThirdParty/glad/include -o glad.o

REM Compile Engine sources
g++ -c ../Engine/Core/Logger.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -o Logger.o
g++ -c ../Engine/Platform/Window.cpp -I../Engine -I../ThirdParty/glad/include -Iglfw/include -DENGINE_DEBUG -o Window.o
g++ -c ../Engine/Renderer/Shader.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -o Shader.o
g++ -c ../Engine/Renderer/Buffer.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -o Buffer.o
g++ -c ../Engine/Renderer/VertexArray.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -o VertexArray.o
g++ -c ../Engine/Renderer/Renderer.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -o Renderer.o
g++ -c ../Engine/Core/Engine.cpp -I../Engine -I../ThirdParty/glad/include -Iglfw/include -DENGINE_DEBUG -o Engine.o

REM Compile example
g++ -c ../Examples/TriangleDemo.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -o TriangleDemo.o

REM Link everything
g++ TriangleDemo.o Engine.o Renderer.o VertexArray.o Buffer.o Shader.o Window.o Logger.o glad.o -Lglfw/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32 -o TriangleDemo.exe

if exist TriangleDemo.exe (
    echo Build successful! Run TriangleDemo.exe to test.
) else (
    echo Build failed!
)

cd ..
pause 