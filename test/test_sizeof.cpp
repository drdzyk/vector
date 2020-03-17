#include <gtest/gtest.h>
#include "vector.hpp"

TEST(VectorTest, sizeof_vector_with_stateless_allocator)
{
    // we employ EBO(empty base optimization) for stateless allocator
    using alloc = std::allocator<int>;
    static_assert(sizeof(alloc) == 1, "std::allocator is stateless");
    static_assert(alignof(low::vector<int, alloc>) == sizeof(void *), "vector aligned by pointer size");
    static_assert(sizeof(low::vector<int, alloc>) == 3 * sizeof(void *), "vector contains 3 pointer");
}

TEST(VectorTest, sizeof_vector_with_statefull_allocator)
{
    struct alloc : std::allocator<int>
    {
        int i{0};
    };
    static_assert(sizeof(alloc) == sizeof(int), "alloc contain state");
    static_assert(alignof(low::vector<int, alloc>) == sizeof(void *), "vector aligned by pointer size");
    static_assert(sizeof(low::vector<int, alloc>) == 3 * sizeof(void *)  + alignof(low::vector<int, alloc>),
        "vector contains 3 pointer + aligned sizeof allocator");
}
