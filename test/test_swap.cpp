#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("swap", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v1;
    SECTION("swap both empty")
    {
        TestType v2;
        v1.swap(v2);
        REQUIRE(v1.empty());
        REQUIRE(v2.empty());
    }
    SECTION("swap one empty")
    {
        TestType v2{7, 8, 9};
        v1.swap(v2);
        REQUIRE(v1 == TestType{7, 8, 9});
        REQUIRE(v2.empty());
    }
    SECTION("swap no one empty")
    {
        v1.emplace_back(11);
        v1.emplace_back(22);
        TestType v2{7, 8, 9};
        v1.swap(v2);
        REQUIRE(v1 == TestType{7, 8, 9});
        REQUIRE(v2 == TestType{11, 22});
    }
}
