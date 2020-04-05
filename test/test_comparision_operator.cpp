#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("comparison operator for equal vectors", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    SECTION("empty vectors equal")
    {
        TestType l, r;
        REQUIRE(l == r);
        REQUIRE_FALSE(l != r);
    }
    SECTION("equal vectors")
    {
        TestType l;
        l.emplace_back(7);
        TestType r = l;
        REQUIRE(l == r);
        REQUIRE_FALSE(l != r);
    }
    SECTION("equal vectors")
    {
        TestType l;
        l.emplace_back(7);
        l.emplace_back(8);
        TestType r = l;
        REQUIRE(l == r);
        REQUIRE_FALSE(l != r);
    }
    SECTION("capacity don't participate in equality")
    {
        TestType l;
        l.emplace_back(7);
        l.emplace_back(8);

        TestType r;
        r.reserve(8);
        r = l;
        REQUIRE(l.capacity() == 2);
        REQUIRE(r.capacity() == 8);
        REQUIRE(l == r);
        REQUIRE_FALSE(l != r);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("comparison operator for unequal vectors", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    SECTION("empty vectors equal")
    {
        TestType l, r;
        l.emplace_back(7);
        REQUIRE_FALSE(l == r);
        REQUIRE(l != r);
    }
    SECTION("equal vectors")
    {
        TestType l, r;
        r.emplace_back(7);
        REQUIRE_FALSE(l == r);
        REQUIRE(l != r);
    }
    SECTION("equal vectors")
    {
        TestType l;
        l.emplace_back(7);
        l.emplace_back(8);
        TestType r = l;
        r.emplace_back(9);

        REQUIRE_FALSE(l == r);
        REQUIRE(l != r);
    }
}
