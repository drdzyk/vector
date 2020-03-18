find_package(GTest REQUIRED)

add_executable(unit_test_old test_constructor.cpp test_move_if_noexcept.cpp test_track_allocations.cpp)
target_link_libraries(unit_test_old PRIVATE ${GTEST_BOTH_LIBRARIES} pthread -fsanitize=address)
target_include_directories(unit_test_old PRIVATE ${CMAKE_SOURCE_DIR})

find_package(Catch2 REQUIRED HINTS /cmake)

add_executable(unit_test test.cpp test_sizeof.cpp)
target_link_libraries(unit_test PRIVATE Catch2::Catch2 Catch2::Catch2Main -fsanitize=address)
target_include_directories(unit_test PRIVATE ${CMAKE_SOURCE_DIR})
