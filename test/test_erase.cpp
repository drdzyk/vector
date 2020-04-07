#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include <array>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("erase on empty vector", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    SECTION("begin")
    {
        v.erase(v.begin(), v.begin());
        REQUIRE(v == TestType{});
    }
    SECTION("end")
    {
        v.erase(v.end(), v.end());
        REQUIRE(v == TestType{});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("erase nothing", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v{1, 2, 3};
    SECTION("begin")
    {
        v.erase(v.begin(), v.begin());
        REQUIRE(v == TestType{1, 2, 3});
    }
    SECTION("middle")
    {
        v.erase(std::next(v.begin()), std::next(v.begin()));
        REQUIRE(v == TestType{1, 2, 3});
    }
    SECTION("middle")
    {
        v.erase(std::next(v.begin(), 2), std::next(v.begin(), 2));
        REQUIRE(v == TestType{1, 2, 3});
    }
    SECTION("end")
    {
        v.erase(std::next(v.end(), 2), std::next(v.end(), 2));
        REQUIRE(v == TestType{1, 2, 3});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("erase", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v{1, 2, 3, 4, 5};
    SECTION("erase all")
    {
        v.erase(v.begin(), v.end());
        REQUIRE(v == TestType{});
    }
    SECTION("erase")
    {
        v.erase(v.begin(), std::next(v.begin()));
        REQUIRE(v == TestType{2, 3, 4, 5});
    }
    SECTION("erase")
    {
        v.erase(v.begin(), std::next(v.begin(), 2));
        REQUIRE(v == TestType{3, 4, 5});
    }
    SECTION("erase")
    {
        v.erase(v.begin(), std::next(v.begin(), 3));
        REQUIRE(v == TestType{4, 5});
    }
    SECTION("erase")
    {
        v.erase(v.begin(), std::next(v.begin(), 4));
        REQUIRE(v == TestType{5});
    }
    SECTION("erase")
    {
        v.erase(std::next(v.begin(), 1), std::next(v.begin(), 4));
        REQUIRE(v == TestType{1, 5});
    }
}
