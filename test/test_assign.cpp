#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include <array>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("assign from empty container", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array<value_type, 0> source{};
    REQUIRE(source.empty());

    SECTION("assign to empty container")
    {
        TestType v;
        v.assign(source.begin(), source.end());
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == 0);
    }
    SECTION("assign to not empty container")
    {
        TestType v;
        v.emplace_back(7);
        REQUIRE(v.size() == 1);
        REQUIRE(v.capacity() == 1);

        v.assign(source.begin(), source.end());
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == 1);
    }
    SECTION("assign to not empty container")
    {
        TestType v{7, 8, 9};
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);

        v.assign(source.begin(), source.end());
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == 3);
    }
    SECTION("assign to not empty container")
    {
        TestType v;
        v.reserve(3);
        v.emplace_back(7);
        REQUIRE(v.size() == 1);
        REQUIRE(v.capacity() == 3);

        v.assign(source.begin(), source.end());
        REQUIRE(v.empty());
        REQUIRE(v.capacity() == 3);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("assign from not empty container", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array<value_type, 3> source{7, 8, 9};
    REQUIRE(source.size() == 3);

    SECTION("assign to empty container")
    {
        TestType v;
        REQUIRE(v.size() == 0);

        v.assign(source.begin(), source.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);
    }
    SECTION("assign to not empty container")
    {
        TestType v;
        v.emplace_back(1);
        v.emplace_back(2);
        REQUIRE(v.size() == 2);
        REQUIRE(v.capacity() == 2);

        v.assign(source.begin(), source.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);
    }
    SECTION("assign to not empty container")
    {
        TestType v;
        v.reserve(3);
        v.emplace_back(11);
        REQUIRE(v.size() == 1);
        REQUIRE(v.capacity() == 3);

        v.assign(source.begin(), source.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);
    }
    SECTION("assign to not empty container")
    {
        TestType v{11, 22, 33};
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);

        v.assign(source.begin(), source.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);
    }
    SECTION("assign to not empty container")
    {
        TestType v;
        v.reserve(4);
        v.emplace_back(11);
        v.emplace_back(22);
        REQUIRE(v.size() == 2);
        REQUIRE(v.capacity() == 4);

        v.assign(source.begin(), source.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 4);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);
    }
    SECTION("assign to not empty container")
    {
        TestType v;
        v.reserve(4);
        v.emplace_back(11);
        v.emplace_back(22);
        v.emplace_back(33);
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 4);

        v.assign(source.begin(), source.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 4);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);
    }
    SECTION("assign to not empty container")
    {
        TestType v{11, 22, 33, 44};
        REQUIRE(v.size() == 4);
        REQUIRE(v.capacity() == 4);

        v.assign(source.begin(), source.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 4);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);
    }
    SECTION("assign to not empty container")
    {
        TestType v;
        v.reserve(5);
        v.emplace_back(11);
        v.emplace_back(22);
        v.emplace_back(33);
        v.emplace_back(44);

        v.assign(source.begin(), source.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 5);
        REQUIRE(v[0] == 7);
        REQUIRE(v[1] == 8);
        REQUIRE(v[2] == 9);
    }
}
