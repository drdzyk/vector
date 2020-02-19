cmake_minimum_required(VERSION 3.10)
project(vector)

add_compile_options(
    -std=c++17
    -g3
    -fsanitize=address
)

add_executable(vector main.cpp)
target_link_libraries(vector -fsanitize=address)
