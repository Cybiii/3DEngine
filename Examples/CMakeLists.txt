cmake_minimum_required(VERSION 3.16)

# Example Applications
project(3DEngine_Examples)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/glad/include)

# Link directories
link_directories(${CMAKE_BINARY_DIR}/Engine)

# Common libraries for all examples
set(EXAMPLE_LIBS Engine glad glfw ${OPENGL_LIBRARIES})

# Example 1: Basic Triangle Demo
add_executable(BasicTriangleDemo BasicTriangleDemo.cpp)
target_link_libraries(BasicTriangleDemo ${EXAMPLE_LIBS})

# Example 2: Animated Triangle Demo  
add_executable(AnimatedTriangleDemo AnimatedTriangleDemo.cpp)
target_link_libraries(AnimatedTriangleDemo ${EXAMPLE_LIBS})

# Example 3: Basic Cube Demo
add_executable(BasicCubeDemo BasicCubeDemo.cpp)
target_link_libraries(BasicCubeDemo ${EXAMPLE_LIBS})

# Copy shaders to build directory
configure_file(${CMAKE_SOURCE_DIR}/Shaders/BasicTriangle.vert ${CMAKE_BINARY_DIR}/Examples/BasicTriangle.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Shaders/BasicTriangle.frag ${CMAKE_BINARY_DIR}/Examples/BasicTriangle.frag COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Shaders/Cube.vert ${CMAKE_BINARY_DIR}/Examples/Cube.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Shaders/Cube.frag ${CMAKE_BINARY_DIR}/Examples/Cube.frag COPYONLY) 