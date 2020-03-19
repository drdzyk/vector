#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("reserve", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    v.reserve(1);
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 1);

    v.emplace_back(77);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 77);
    REQUIRE(v.capacity() == 1);

    v.reserve(11);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 77);
    REQUIRE(v.capacity() == 11);

    v.reserve(11);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 77);
    REQUIRE(v.capacity() == 11);

    v.reserve(7);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 77);
    REQUIRE(v.capacity() == 11);

    for (std::size_t idx{0}; idx < 10; ++idx)
    {
        v.emplace_back(idx);
        REQUIRE(v.size() == idx + 2);
        REQUIRE(v.capacity() == 11);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("empty, size", "[low::vector][std::vector]",
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
