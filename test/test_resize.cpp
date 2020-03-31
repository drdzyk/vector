#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("resize", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    using value_type =  typename TestType::value_type;

    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);

    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);
    REQUIRE(v[2] == 9);

    v.resize(1);
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);

    v.resize(3);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{});
    REQUIRE(v[2] == value_type{});

    v.resize(4);
    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{});
    REQUIRE(v[2] == value_type{});
    REQUIRE(v[3] == value_type{});

    v.resize(7);
    REQUIRE(v.size() == 7);
    REQUIRE(v.capacity() == 8);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{});
    REQUIRE(v[2] == value_type{});
    REQUIRE(v[3] == value_type{});
    REQUIRE(v[4] == value_type{});
    REQUIRE(v[5] == value_type{});
    REQUIRE(v[6] == value_type{});
}

TEMPLATE_PRODUCT_TEST_CASE("resize with value", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    using value_type =  typename TestType::value_type;

    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);

    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);
    REQUIRE(v[2] == 9);

    v.resize(1, value_type{777});
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);

    v.resize(3, value_type{666});
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{666});
    REQUIRE(v[2] == value_type{666});

    v.resize(4);
    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{666});
    REQUIRE(v[2] == value_type{666});
    REQUIRE(v[3] == value_type{});

    v.resize(7, value_type{999});
    REQUIRE(v.size() == 7);
    REQUIRE(v.capacity() == 8);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{666});
    REQUIRE(v[2] == value_type{666});
    REQUIRE(v[3] == value_type{});
    REQUIRE(v[4] == value_type{999});
    REQUIRE(v[5] == value_type{999});
    REQUIRE(v[6] == value_type{999});
}

TEMPLATE_PRODUCT_TEST_CASE("resize scale", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;

    v.resize(8);
    REQUIRE(v.size() == 8);
    REQUIRE(v.capacity() == 8);

    v.resize(0);
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 8);

    v.resize(7);
    REQUIRE(v.size() == 7);
    REQUIRE(v.capacity() == 8);

    v.resize(11);
    REQUIRE(v.size() == 11);
    REQUIRE(v.capacity() == 14);

    v.resize(23);
    REQUIRE(v.size() == 23);
    REQUIRE(v.capacity() == 23);
}
