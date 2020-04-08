#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("insert copy to end, not enough capacity", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    SECTION("insert to empty container")
    {
        TestType v;
        v.insert(v.end(), 3, 7);
        REQUIRE(v == TestType{7, 7, 7});
    }
    SECTION("insert to not empty container")
    {
        TestType v;
        v.emplace_back(11);
        v.insert(v.end(), 3, 7);
        REQUIRE(v == TestType{11, 7, 7, 7});
    }
    SECTION("insert to not empty container")
    {
        TestType v;
        v.emplace_back(11);
        v.emplace_back(22);
        v.insert(v.end(), 3, 7);
        REQUIRE(v == TestType{11, 22, 7, 7, 7});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert copy to end, not enough capacity 2", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    SECTION("insert to empty container")
    {
        TestType v;
        v.reserve(1);
        v.insert(v.end(), 3, 7);
        REQUIRE(v == TestType{7, 7, 7});
    }
    SECTION("insert to not empty container")
    {
        TestType v;
        v.reserve(2);
        v.emplace_back(11);
        v.insert(v.end(), 3, 7);
        REQUIRE(v == TestType{11, 7, 7, 7});
    }
    SECTION("insert to not empty container")
    {
        TestType v;
        v.reserve(4);
        v.emplace_back(11);
        v.emplace_back(22);
        v.insert(v.end(), 3, 7);
        REQUIRE(v == TestType{11, 22, 7, 7, 7});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert copy, not enough capacity", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin(), 2, 7);
        REQUIRE(v == TestType{7, 7, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()), 2, 7);
        REQUIRE(v == TestType{11, 7, 7, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2), 2, 7);
        REQUIRE(v == TestType{11, 22, 7, 7, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3), 2, 7);
        REQUIRE(v == TestType{11, 22, 33, 7, 7, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), 2, 7);
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 7});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert copy, enough capacity", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    v.reserve(5);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin(), 1, 7);
        REQUIRE(v == TestType{7, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()), 1, 7);
        REQUIRE(v == TestType{11, 7, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2), 1, 7);
        REQUIRE(v == TestType{11, 22, 7, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3), 1, 7);
        REQUIRE(v == TestType{11, 22, 33, 7, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), 1, 7);
        REQUIRE(v == TestType{11, 22, 33, 44, 7});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert copy, enough capacity 1", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    v.reserve(7);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin(), 2, 7);
        REQUIRE(v == TestType{7, 7, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()), 2, 7);
        REQUIRE(v == TestType{11, 7, 7, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2), 2, 7);
        REQUIRE(v == TestType{11, 22, 7, 7, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3), 2, 7);
        REQUIRE(v == TestType{11, 22, 33, 7, 7, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), 2, 7);
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 7});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert copy, enough capacity 2", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    v.reserve(7);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin(), 3, 7);
        REQUIRE(v == TestType{7, 7, 7, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()), 3, 7);
        REQUIRE(v == TestType{11, 7, 7, 7, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2), 3, 7);
        REQUIRE(v == TestType{11, 22, 7, 7, 7, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3), 3, 7);
        REQUIRE(v == TestType{11, 22, 33, 7, 7, 7, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), 3, 7);
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 7, 7});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert copy, enough capacity 3", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    v.reserve(8);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin(), 4, 7);
        REQUIRE(v == TestType{7, 7, 7, 7, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()), 4, 7);
        REQUIRE(v == TestType{11, 7, 7, 7, 7, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2), 4, 7);
        REQUIRE(v == TestType{11, 22, 7, 7, 7, 7, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3), 4, 7);
        REQUIRE(v == TestType{11, 22, 33, 7, 7, 7, 7, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), 4, 7);
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 7, 7, 7});
    }
}
