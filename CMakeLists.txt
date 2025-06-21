cmake_minimum_required(VERSION 3.16)
project(3DEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Enable testing
enable_testing()

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find packages
find_package(OpenGL REQUIRED)

# Add subdirectories
add_subdirectory(ThirdParty)
add_subdirectory(Engine)
add_subdirectory(Examples)
add_subdirectory(Tests)

# Create Assets directory if it doesn't exist
if(NOT EXISTS ${CMAKE_BINARY_DIR}/Assets)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Assets)
endif()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/Shaders)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Shaders)
endif()

# Copy assets and shaders to build directory if they exist
if(EXISTS ${CMAKE_SOURCE_DIR}/Assets)
    file(COPY ${CMAKE_SOURCE_DIR}/Assets/ DESTINATION ${CMAKE_BINARY_DIR}/Assets/)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/Shaders)
    file(COPY ${CMAKE_SOURCE_DIR}/Shaders/ DESTINATION ${CMAKE_BINARY_DIR}/Shaders/)
endif() 