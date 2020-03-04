cmake_minimum_required(VERSION 3.10)
project(vector)

add_compile_options(
    -std=c++17
    -g3
    -fsanitize=address
)

add_executable(vector main.cpp)
target_link_libraries(vector -fsanitize=address)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(unit_test test.cpp test_exceptions.cpp)
target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES} gtest -fsanitize=address  pthread)
