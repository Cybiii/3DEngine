@echo off
echo ========================================
echo  Building 3D Engine Examples
echo ========================================

REM Check if build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

echo.
echo Configuring CMake...
cmake -G "MinGW Makefiles" ..
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b %errorlevel%
)

echo.
echo Building project...
mingw32-make
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo.
echo ========================================
echo  Examples built successfully!
echo ========================================
echo.
echo Available examples:
echo - Examples\SpinningTriangle.exe
echo - Examples\RotatingCube.exe  
echo - Examples\MultipleCubes.exe
echo - Examples\CameraDemo.exe
echo - Examples\MathPlayground.exe
echo.
echo Run examples from the build directory.
echo.
pause 