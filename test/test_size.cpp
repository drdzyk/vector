#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("empty and size", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);

    v.emplace_back(7);
    REQUIRE_FALSE(v.empty());
    REQUIRE(v.size() == 1);

    v.clear();
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
}
