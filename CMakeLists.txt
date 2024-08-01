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

add_test(NAME UnitTests COMMAND unit_test)
add_test(NAME IntegrationTests COMMAND integration_test)
add_test(NAME E2ETests COMMAND e2e_test)
