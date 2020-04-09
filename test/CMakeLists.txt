find_package(Catch2 REQUIRED HINTS /cmake)

add_executable(unit_test
    test_assign.cpp
    test_assign_tracker.cpp
    test_clear.cpp
    test_comparision_operator.cpp
    test_copy_assignment_operator.cpp
    test_copy_assignment_operator_tracker.cpp
    test_copy_constructor.cpp
    test_copy_constructor_tracker.cpp
    test_emplace_back.cpp
    test_erase.cpp
    test_erase_tracker.cpp
    test_initializer_list_constructor.cpp
    test_insert.cpp
    test_insert_copy.cpp
    test_insert_copy_tracker.cpp
    test_insert_tracker.cpp
    test_iterators.cpp
    test_move_assignment_operator.cpp
    test_move_assignment_operator_tracker.cpp
    test_move_constructor.cpp
    test_move_constructor_tracker.cpp
    test_move_if_noexcept.cpp
    test_noexcept_specifier.cpp
    test_pop_back.cpp
    test_reserve.cpp
    test_resize.cpp
    test_shrink_to_fit.cpp
    test_size.cpp
    test_sizeof.cpp
    test_track_allocations.cpp
    Allocator.cpp
)
target_link_libraries(unit_test PRIVATE Catch2::Catch2 Catch2::Catch2Main -fsanitize=address)
target_include_directories(unit_test PRIVATE ${CMAKE_SOURCE_DIR})
