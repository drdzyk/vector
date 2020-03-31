#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>
#include "vector.hpp"

TEST_CASE("sizeof vector with stateless allocator", "[vector]")
{
    // we employ EBO(empty base optimization) for stateless allocator
    using alloc = std::allocator<int>;
    static_assert(std::is_empty_v<alloc>, "std::allocator is stateless");
    static_assert(alignof(low::vector<int, alloc>) == sizeof(void *), "vector aligned by pointer size");
    static_assert(sizeof(low::vector<int, alloc>) == 3 * sizeof(void *), "vector contains 3 pointer");
}

TEST_CASE("sizeof vector with stateful allocator", "[vector]")
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
