cmake_minimum_required(VERSION 3.10)
project(vector LANGUAGES CXX)

add_compile_options(
    -std=c++17
    -g3
    -Wall
    -Werror
    -Wextra
    -fsanitize=address
)

add_executable(main main.cpp)
target_link_libraries(main -fsanitize=address)

add_subdirectory(test)
add_subdirectory(benchmark EXCLUDE_FROM_ALL)
