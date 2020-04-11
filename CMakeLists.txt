cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER clang++-9)
project(vector LANGUAGES CXX)

add_compile_options(
    -std=c++2a
    -g3
    -Wall
    -Werror
    -Wextra
    -Wconversion
    -fsanitize=address
)

enable_testing()

add_subdirectory(test)
add_subdirectory(benchmark EXCLUDE_FROM_ALL)
