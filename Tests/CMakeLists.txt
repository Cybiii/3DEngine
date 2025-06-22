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

# Test executables
add_executable(Phase1IntegrationTests Phase1IntegrationTests.cpp)
add_executable(Phase2MathTests Phase2MathTests.cpp)

# Link test executables to the engine
target_link_libraries(Phase1IntegrationTests PRIVATE Engine)
target_link_libraries(Phase2MathTests PRIVATE Engine)

# Include engine headers
target_include_directories(Phase1IntegrationTests PRIVATE ${CMAKE_SOURCE_DIR}/Engine)
target_include_directories(Phase2MathTests PRIVATE ${CMAKE_SOURCE_DIR}/Engine)

# Enable testing
enable_testing()

# Add tests to CTest
add_test(NAME Phase1Integration COMMAND Phase1IntegrationTests)
add_test(NAME Phase2MathFoundation COMMAND Phase2MathTests) 