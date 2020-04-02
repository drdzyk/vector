#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>
#include "vector.hpp"
#include "Allocator.hpp"

TEST_CASE("trivial constructors noexcept specifiers", "[vector]")
{
    using alloc = alloc::NotEq<int>;
    using vec = low::vector<int, alloc>;

    static_assert(noexcept(vec{}));
    static_assert(noexcept(vec{alloc{}}));
    static_assert(noexcept(vec{vec{}}));

    vec lvalue_vector;
    static_assert(!noexcept(vec{lvalue_vector}));
    static_assert(!noexcept(vec{lvalue_vector, alloc{}}));

    static_assert(!noexcept(vec{1, 2, 3}), "initializer list ctor may throw");
}

TEST_CASE("allocator-extended move constructor noexcept with always equals allocators", "[vector]")
{
    using alloc = alloc::Eq<int>;
    using vec = low::vector<int, alloc>;
    static_assert(noexcept(vec{vec{}, alloc{}}));
}

TEST_CASE("allocator-extended move constructor may throws with not always equals allocators", "[vector]")
{
    using alloc = alloc::NotEq<int>;
    using vec = low::vector<int, alloc>;
    static_assert(!noexcept(vec{vec{}, alloc{}}));
}
