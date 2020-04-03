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

add_executable(main main.cpp)
target_link_libraries(main -fsanitize=address)

add_subdirectory(test)
add_subdirectory(benchmark EXCLUDE_FROM_ALL)
