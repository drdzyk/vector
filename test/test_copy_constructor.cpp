#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("copy constructor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType source;
    REQUIRE(source.empty());

    SECTION("copy from empty source produce empty container")
    {
        auto copy = std::move(source);
        REQUIRE(copy.empty());
    }

    SECTION("copy produce vector with same content")
    {
        source.emplace_back(7);
        source.emplace_back(8);

        auto copy = source;
        REQUIRE(copy[0] == 7);
        REQUIRE(copy[1] == 8);
    }
}
