# Examples
add_executable(TriangleDemo
    TriangleDemo.cpp
)

target_link_libraries(TriangleDemo
    PRIVATE 
        Engine
)

# Copy executable to build directory
set_target_properties(TriangleDemo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
) 