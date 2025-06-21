# Engine Library
add_library(Engine STATIC)

# Engine source files
target_sources(Engine PRIVATE
    # Core
    Core/Engine.cpp
    Core/Logger.cpp
    
    # Platform
    Platform/Window.cpp
    
    # Renderer
    Renderer/Renderer.cpp
    Renderer/Shader.cpp
    Renderer/Buffer.cpp
    Renderer/VertexArray.cpp
)

# Engine headers
target_sources(Engine PRIVATE
    # Core headers
    Core/Engine.h
    Core/Logger.h
    
    # Platform headers  
    Platform/Window.h
    
    # Renderer headers
    Renderer/Renderer.h
    Renderer/Shader.h
    Renderer/Buffer.h
    Renderer/VertexArray.h
)

# Include directories
target_include_directories(Engine 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Link libraries
target_link_libraries(Engine 
    PUBLIC 
        OpenGL::GL
        ThirdParty::GLFW
        ThirdParty::GLAD
        ThirdParty::GLM
)

# Compiler features
target_compile_features(Engine PUBLIC cxx_std_17)

# Preprocessor definitions
target_compile_definitions(Engine 
    PRIVATE
        ENGINE_BUILD
    PUBLIC
        $<$<CONFIG:Debug>:ENGINE_DEBUG>
        $<$<CONFIG:Release>:ENGINE_RELEASE>
) 