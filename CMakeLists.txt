cmake_minimum_required(VERSION 3.21)
project(Minesweeper DESCRIPTION "MMO Minesweeper game in C")
remove_definitions(-DNDEBUG)

include_directories(src tests)

add_library(minesweeper src/minesweeper.c src/minesweeper.h)

add_executable(ms src/main.c)
target_link_libraries(ms minesweeper)

add_executable(unit_test tests/unit.c)
target_link_libraries(unit_test minesweeper)

add_executable(integration_test tests/integration.c)
target_link_libraries(integration_test minesweeper)

add_executable(e2e_test tests/e2e.c)
target_link_libraries(e2e_test minesweeper)

enable_testing()

# Add tests to CTest
add_test(NAME UnitTests COMMAND unit_test)
add_test(NAME IntegrationTests COMMAND integration_test)
add_test(NAME E2ETests COMMAND e2e_test)

# Define Dr. Memory path
find_program(DRMEMORY_EXECUTABLE drmemory)

# Custom command to run Dr. Memory on all test executables
add_custom_target(
    test_with_drmemory
    COMMAND ${DRMEMORY_EXECUTABLE} -- ${CMAKE_BINARY_DIR}/unit_test
    COMMAND ${DRMEMORY_EXECUTABLE} -- ${CMAKE_BINARY_DIR}/integration_test
    COMMAND ${DRMEMORY_EXECUTABLE} -- ${CMAKE_BINARY_DIR}/e2e_test
    COMMENT "Running all tests with Dr. Memory..."
)

# Add a custom command to run tests with Dr. Memory
add_custom_command(
    TARGET test_with_drmemory
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target test_with_drmemory
    COMMENT "Running all tests with Dr. Memory after build..."
)
