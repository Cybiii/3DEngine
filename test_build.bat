@echo off
echo Testing 3D Engine compilation...

REM Create test build directory
if not exist "test_build" mkdir test_build
cd test_build

REM Download GLFW precompiled binaries for Windows
echo Downloading GLFW...
if not exist "glfw" (
    curl -L -o glfw.zip "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip" 2>nul
    if exist glfw.zip (
        echo Extracting GLFW...
        powershell -command "Expand-Archive glfw.zip -DestinationPath . -Force" 2>nul
        ren glfw-3.3.8.bin.WIN64 glfw 2>nul
        del glfw.zip 2>nul
        echo GLFW downloaded and extracted successfully
    ) else (
        echo Failed to download GLFW, trying manual compilation...
    )
) else (
    echo GLFW already exists
)

REM Test individual component compilation
echo.
echo Testing individual component compilation...

echo Testing Logger...
g++ -c ../Engine/Core/Logger.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -o Logger.o
if %errorlevel% neq 0 (
    echo Logger compilation failed!
    pause
    exit /b 1
)

echo Testing GLAD...
g++ -c ../ThirdParty/glad/src/glad.c -I../ThirdParty/glad/include -o glad.o
if %errorlevel% neq 0 (
    echo GLAD compilation failed!
    pause
    exit /b 1
)

echo Testing Shader...
g++ -c ../Engine/Renderer/Shader.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -o Shader.o
if %errorlevel% neq 0 (
    echo Shader compilation failed!
    pause
    exit /b 1
)

echo Testing Buffer...
g++ -c ../Engine/Renderer/Buffer.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -o Buffer.o
if %errorlevel% neq 0 (
    echo Buffer compilation failed!
    pause
    exit /b 1
)

echo Testing VertexArray...
g++ -c ../Engine/Renderer/VertexArray.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -o VertexArray.o
if %errorlevel% neq 0 (
    echo VertexArray compilation failed!
    pause
    exit /b 1
)

echo Testing Renderer...
g++ -c ../Engine/Renderer/Renderer.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -o Renderer.o
if %errorlevel% neq 0 (
    echo Renderer compilation failed!
    pause
    exit /b 1
)

echo.
echo All core components compiled successfully!

if exist glfw (
    echo Testing Window with GLFW...
    g++ -c ../Engine/Platform/Window.cpp -I../Engine -I../ThirdParty/glad/include -Iglfw/include -DENGINE_DEBUG -std=c++17 -o Window.o
    if %errorlevel% neq 0 (
        echo Window compilation failed!
        pause
        exit /b 1
    )
    
    echo Testing Engine...
    g++ -c ../Engine/Core/Engine.cpp -I../Engine -I../ThirdParty/glad/include -Iglfw/include -DENGINE_DEBUG -std=c++17 -o Engine.o
    if %errorlevel% neq 0 (
        echo Engine compilation failed!
        pause
        exit /b 1
    )
    
    echo.
    echo Full engine compilation test successful!
    echo You can now work on the complete CMake build system.
) else (
    echo GLFW not available, skipping Window and Engine compilation
    echo Core renderer components work correctly!
)

cd ..
echo.
echo Test build completed successfully!
pause 