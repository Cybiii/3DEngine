# Tests
add_executable(Phase1Tests
    Phase1IntegrationTests.cpp
)

target_link_libraries(Phase1Tests
    PRIVATE 
        Engine
)

# Register tests with CTest
add_test(NAME Phase1_Integration COMMAND Phase1Tests)

# Copy executable to build directory
set_target_properties(Phase1Tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
) 