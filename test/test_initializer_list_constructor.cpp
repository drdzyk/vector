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
    {
        std::initializer_list<value_type> list;
        TestType v{list};
        REQUIRE(v.empty());
    }
    {
        std::initializer_list<value_type> list{7};
        TestType v{list};
        REQUIRE(v.size() == 1);
        REQUIRE(v.capacity() == 1);
        REQUIRE(v[0] == 7);
    }
    {
        std::initializer_list<value_type> list{7, 8};
        TestType v{list};
        REQUIRE(v.size() == 2);
        REQUIRE(v.capacity() == 2);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
    }
    {
        TestType v{1, 2, 3};
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(v[0] == 1);
        REQUIRE(v[1] == 2);
        REQUIRE(v[2] == 3);
    }
}
