# Third-party libraries

# GLFW - Window management
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Download GLFW
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# GLAD - OpenGL loading
add_library(glad STATIC)
target_sources(glad PRIVATE
    glad/src/glad.c
)
target_include_directories(glad PUBLIC
    glad/include
)

# Create GLAD files if they don't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/glad/include/KHR)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/glad/src)

# GLM - Math library (use newer version that's compatible)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# Create alias targets for easier linking
add_library(ThirdParty::GLFW ALIAS glfw)
add_library(ThirdParty::GLAD ALIAS glad)
add_library(ThirdParty::GLM ALIAS glm) 