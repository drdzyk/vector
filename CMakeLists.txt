cmake_minimum_required(VERSION 3.10)
project(vector)

add_compile_options(
    -std=c++17
    -g3
    -fsanitize=address
)

add_executable(main main.cpp)
target_link_libraries(main -fsanitize=address)

find_package(GTest REQUIRED)

add_executable(unit_test test/test.cpp test/test_move_if_noexcept.cpp test/test_track_allocations.cpp)
target_link_libraries(unit_test PRIVATE ${GTEST_BOTH_LIBRARIES} pthread -fsanitize=address)
target_include_directories(unit_test PRIVATE ${CMAKE_SOURCE_DIR})
