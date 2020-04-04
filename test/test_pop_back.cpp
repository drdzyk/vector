#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("pop_back", "[low::vector][std::vector]",
    (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v{7, 8, 9};
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 3);

    v.pop_back();
    REQUIRE(v.size() == 2);
    REQUIRE(v.capacity() == 3);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);

    v.pop_back();
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 3);
    REQUIRE(v[0] == 7);

    v.pop_back();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 3);

    v.emplace_back(33);
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 3);
    REQUIRE(v[0] == 33);

    v.pop_back();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 3);
}
