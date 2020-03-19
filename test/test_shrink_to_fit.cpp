#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("shrink_to_fit", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;

    v.shrink_to_fit();
    v.shrink_to_fit(); // double shrink_to_fit() is ok

    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);

    v.shrink_to_fit();
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 3);

    v.shrink_to_fit(); // nothing changed
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 3);

    v.clear();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 3);

    v.shrink_to_fit();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    v.shrink_to_fit(); // double shrink_to_fit() is ok
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}
