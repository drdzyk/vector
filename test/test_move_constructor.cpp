#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"
#include "Allocator.hpp"

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

TEMPLATE_PRODUCT_TEST_CASE("allocator-extended move constructor", "[low::vector][std::vector]",
    (low::vector, std::vector), (
        (int, alloc::WrongEq<int>),
        (int, alloc::StaticEq<int>),
        (double, alloc::StaticEq<double>),
        (DynamicInt, alloc::StaticEq<DynamicInt>),
        (int, alloc::DynamicEq<int>),
        (double, alloc::DynamicEq<double>),
        (DynamicInt, alloc::DynamicEq<DynamicInt>),
        (int, alloc::DynamicNotEq<int>),
        (double, alloc::DynamicNotEq<double>),
        (DynamicInt, alloc::DynamicNotEq<DynamicInt>)
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
