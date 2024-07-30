cmake_minimum_required(VERSION 3.21)
project(Minesweeper DESCRIPTION "MMO Minesweeper game in C")

add_executable(ms src/minesweeper.c src/main.c)

add_executable(unit_test tests/unit.c)
add_executable(integration_test tests/integration.c)
add_executable(e2e_test tests/e2e.c)

enable_testing()

add_test(NAME UnitTests COMMAND unit_test)
add_test(NAME IntegrationTests COMMAND integration_test)
add_test(NAME E2ETests COMMAND e2e_test)
