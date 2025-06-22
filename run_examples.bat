@echo off
echo ========================================
echo  3D Engine Examples Menu
echo ========================================
echo.
echo Available Examples:
echo 1. Basic Triangle Demo
echo 2. Animated Triangle Demo  
echo 3. Basic Cube Demo
echo 4. Exit
echo.

:menu
set /p choice="Enter your choice (1-4): "

if "%choice%"=="1" (
    echo.
    echo 🔺 Running Basic Triangle Demo...
    echo Press ESC to exit the demo
    echo.
    cd build\Examples
    BasicTriangleDemo.exe
    cd ..\..
    echo.
    goto menu
)

if "%choice%"=="2" (
    echo.
    echo 🔺 Running Animated Triangle Demo...
    echo Press ESC to exit the demo
    echo.
    cd build\Examples
    AnimatedTriangleDemo.exe
    cd ..\..
    echo.
    goto menu
)

if "%choice%"=="3" (
    echo.
    echo 🎲 Running Basic Cube Demo...
    echo Press ESC to exit the demo
    echo.
    cd build\Examples
    BasicCubeDemo.exe
    cd ..\..
    echo.
    goto menu
)

if "%choice%"=="4" (
    echo.
    echo 👋 Goodbye!
    echo.
    pause
    exit /b 0
)

echo Invalid choice. Please try again.
echo.
goto menu 