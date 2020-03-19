find_package(Catch2 REQUIRED HINTS /cmake)

add_executable(unit_test
    test.cpp
    test_constructor.cpp
    test_emplace_back.cpp
    test_iterators.cpp
    test_move_if_noexcept.cpp
    test_resize.cpp
    test_sizeof.cpp
    test_shrink_to_fit.cpp
    test_track_allocations.cpp
)
target_link_libraries(unit_test PRIVATE Catch2::Catch2 Catch2::Catch2Main -fsanitize=address)
target_include_directories(unit_test PRIVATE ${CMAKE_SOURCE_DIR})
