#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("vector fill constructors", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;

    SECTION("zero count")
    {
        TestType v(0, 7);
        REQUIRE(v.empty());
    }
    SECTION("not zero count constructor")
    {
        TestType v(3, 7);
        REQUIRE(v == TestType{7, 7, 7});
    }

    SECTION("zero count, default initialize constructor")
    {
        TestType v(0);
        REQUIRE(v.empty());
    }
    SECTION("default initialize constructor")
    {
        TestType v(3);
        REQUIRE(v == TestType{value_type{}, value_type{}, value_type{}});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("vector iterator constructor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;

    SECTION("empty source")
    {
        const std::array<value_type, 0> source{};
        TestType v(source.begin(), source.end());
        REQUIRE(v.empty());
    }
    SECTION("not empty source")
    {
        const std::array source{value_type{7}, value_type{8}, value_type{9}};
        TestType v(source.begin(), source.end());
        REQUIRE(v == TestType{7, 8, 9});
    }
}
