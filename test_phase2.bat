@echo off
echo ========================================
echo  Phase 2 Math Foundation Tests
echo ========================================

cd test_build

echo Compiling Math Tests...

REM Compile all math components
g++ -c ../Engine/Math/MathTypes.h -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -msse4.1 -o MathTypes.o 2>nul
g++ -c ../Engine/Math/Vector.h -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -msse4.1 -o Vector.o 2>nul
g++ -c ../Engine/Math/Matrix.h -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -msse4.1 -o Matrix.o 2>nul
g++ -c ../Engine/Math/Quaternion.h -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -msse4.1 -o Quaternion.o 2>nul

REM Compile Phase 2 tests
g++ -c ../Tests/Phase2MathTests.cpp -I../Engine -I../ThirdParty/glad/include -DENGINE_DEBUG -std=c++17 -msse4.1 -o Phase2MathTests.o

if %errorlevel% neq 0 (
    echo Failed to compile Phase 2 tests!
    pause
    exit /b 1
)

REM Link Phase 2 tests (no OpenGL needed for math tests)
g++ Phase2MathTests.o Logger.o -o Phase2MathTests.exe

if %errorlevel% neq 0 (
    echo Failed to link Phase 2 tests!
    pause
    exit /b 1
)

echo ========================================
echo  Running Phase 2 Math Tests...
echo ========================================

Phase2MathTests.exe

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo  🎉 Phase 2 Math Tests PASSED! 🎉
    echo ========================================
    echo  ✅ SIMD Vector Math Working
    echo  ✅ Matrix Transformations Working  
    echo  ✅ Quaternion Rotations Working
    echo  ✅ Transform System Working
    echo  ✅ Camera System Working
    echo  🚀 Ready for 3D Cube Demo!
    echo ========================================
) else (
    echo.
    echo ========================================
    echo  ❌ Phase 2 Math Tests FAILED! ❌
    echo ========================================
)

pause 