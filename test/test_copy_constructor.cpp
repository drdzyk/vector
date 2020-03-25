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
        auto copy = source;
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

TEMPLATE_PRODUCT_TEST_CASE("allocator-extended copy constructor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using allocator_type = typename TestType::allocator_type;

    TestType source;
    REQUIRE(source.empty());

    SECTION("copy from empty source produce empty container")
    {
        allocator_type other;
        TestType copy{source, other};
        REQUIRE(copy.empty());
    }

    SECTION("copy produce vector with same content")
    {
        source.emplace_back(7);
        source.emplace_back(8);

        allocator_type other;
        TestType copy{source, other};
        REQUIRE(copy[0] == 7);
        REQUIRE(copy[1] == 8);
    }
}
