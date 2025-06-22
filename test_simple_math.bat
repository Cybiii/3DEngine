@echo off
echo ========================================
echo  Simple Math Foundation Tests  
echo ========================================

cd test_build

echo Compiling Simple Math Tests...

REM Compile simple math tests (only needs Logger)
g++ -c ../Tests/SimpleMathTests.cpp -I../Engine -DENGINE_DEBUG -std=c++17 -o SimpleMathTests.o

if %errorlevel% neq 0 (
    echo Failed to compile simple math tests!
    pause
    exit /b 1
)

REM Link simple tests
g++ SimpleMathTests.o Logger.o -o SimpleMathTests.exe

if %errorlevel% neq 0 (
    echo Failed to link simple math tests!
    pause
    exit /b 1
)

echo ========================================
echo  Running Simple Math Tests...
echo ========================================

SimpleMathTests.exe

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo  🎉 MATH FOUNDATION VALIDATED! 🎉
    echo ========================================
    echo  ✅ Vector Operations Working
    echo  ✅ Matrix Operations Working  
    echo  ✅ Basic Math Validated
    echo  🚀 Ready for Phase 2 Implementation!
    echo ========================================
) else (
    echo.
    echo ========================================
    echo  ❌ Math Tests Failed! ❌
    echo ========================================
)

pause 