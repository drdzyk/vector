#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("move constructor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType source;
    REQUIRE(source.empty());

    SECTION("move from empty source produce empty container")
    {
        auto copy = std::move(source);
        REQUIRE(copy.empty());
    }

    SECTION("move produce vector with same content")
    {
        source.emplace_back(7);
        source.emplace_back(8);

        auto copy = std::move(source);
        REQUIRE(copy[0] == 7);
        REQUIRE(copy[1] == 8);
    }
}

namespace
{
    template <typename T>
    struct ComparableAllocator
    {
        using value_type = T;

        T *allocate(std::size_t n) { return std::allocator<T>{}.allocate(n); }
        void deallocate(T *p, std::size_t s) { std::allocator<T>{}.deallocate(p, s); }
    };

    template <typename T>
    struct EqualAllocator : ComparableAllocator<T>
    {
        friend bool operator==(const EqualAllocator&, const EqualAllocator&) noexcept { return true; }
        friend bool operator!=(const EqualAllocator&l, const EqualAllocator&r) noexcept { return !(l == r); }
    };

    template <typename T>
    struct NotEqualAllocator : ComparableAllocator<T>
    {
        friend bool operator==(const NotEqualAllocator&, const NotEqualAllocator&) noexcept { return false; }
        friend bool operator!=(const NotEqualAllocator&l, const NotEqualAllocator&r) noexcept { return !(l == r); }
    };
}

TEMPLATE_PRODUCT_TEST_CASE("allocator-extended move constructor", "[low::vector][std::vector]",
    (low::vector, std::vector), (
        (int, std::allocator<int>),
        (double, std::allocator<double>),
        (DynamicInt, std::allocator<DynamicInt>),
        (int, NotEqualAllocator<int>),
        (double, NotEqualAllocator<double>),
        (DynamicInt, NotEqualAllocator<DynamicInt>)
    )
)
{
    TestType source;
    REQUIRE(source.empty());

    SECTION("move from empty source produce empty container")
    {
        typename TestType::allocator_type other;
        TestType copy{std::move(source), other};
        REQUIRE(copy.empty());
    }

    SECTION("move produce vector with same content")
    {
        source.emplace_back(7);
        source.emplace_back(8);

        typename TestType::allocator_type other;
        TestType copy{std::move(source), other};
        REQUIRE(copy[0] == 7);
        REQUIRE(copy[1] == 8);
    }
}
