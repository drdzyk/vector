#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("vector initializer_list constructor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    using init_list = std::initializer_list<value_type>;

    SECTION("empty initializer_list")
    {
        TestType v{init_list{}};
        REQUIRE(v.empty());
    }
    SECTION("initializer_list with 1 element")
    {
        TestType v{init_list{7}};
        REQUIRE(v.size() == 1);
        REQUIRE(v.capacity() == 1);
        REQUIRE(v[0] == 7);
    }
    SECTION("initializer_list with 2 elements")
    {
        TestType v{init_list{7, 8}};
        REQUIRE(v.size() == 2);
        REQUIRE(v.capacity() == 2);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
    }
    SECTION("initializer_list with 3 elements")
    {
        TestType v{1, 2, 3};
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
}
