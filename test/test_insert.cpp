#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include <array>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("insert to end, not enough capacity", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array source{value_type{7}, value_type{8}, value_type{9}};

    SECTION("insert to empty container")
    {
        TestType v;
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{7, 8, 9});
    }
    SECTION("insert to not empty container")
    {
        TestType v;
        v.emplace_back(11);
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 7, 8, 9});
    }
    SECTION("insert to not empty container")
    {
        TestType v;
        v.emplace_back(11);
        v.emplace_back(22);
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 7, 8, 9});
    }
}
TEMPLATE_PRODUCT_TEST_CASE("insert to end, not enough capacity 2", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array source{value_type{7}, value_type{8}, value_type{9}};

    SECTION("insert to empty container")
    {
        TestType v;
        v.reserve(1);
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{7, 8, 9});
    }
    SECTION("insert to not empty container")
    {
        TestType v;
        v.reserve(2);
        v.emplace_back(11);
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 7, 8, 9});
    }
    SECTION("insert to not empty container")
    {
        TestType v;
        v.reserve(4);
        v.emplace_back(11);
        v.emplace_back(22);
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 7, 8, 9});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert, not enough capacity", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array source{value_type{7}, value_type{8}};

    TestType v;
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin() , source.begin(), source.end());
        REQUIRE(v == TestType{7, 8, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 7, 8, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 7, 8, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 7, 8, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 8});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert, enough capacity", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array source{value_type{7}};

    TestType v;
    v.reserve(5);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin() , source.begin(), source.end());
        REQUIRE(v == TestType{7, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 7, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 7, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 7, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 44, 7});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert, enough capacity 1", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array source{value_type{7}, value_type{8}};

    TestType v;
    v.reserve(7);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin() , source.begin(), source.end());
        REQUIRE(v == TestType{7, 8, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 7, 8, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 7, 8, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 7, 8, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 8});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert, enough capacity 2", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array source{value_type{7}, value_type{8}, value_type{9}};

    TestType v;
    v.reserve(7);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin() , source.begin(), source.end());
        REQUIRE(v == TestType{7, 8, 9, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 7, 8, 9, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 7, 8, 9, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 7, 8, 9, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 8, 9});
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert, enough capacity 3", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    const std::array source{value_type{7}, value_type{8}, value_type{9}, value_type{10}};

    TestType v;
    v.reserve(8);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin() , source.begin(), source.end());
        REQUIRE(v == TestType{7, 8, 9, 10, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 7, 8, 9, 10, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 7, 8, 9, 10, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 7, 8, 9, 10, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 8, 9, 10});
    }
}


TEMPLATE_PRODUCT_TEST_CASE("insert std::initializer_list overload", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    using value_type = typename TestType::value_type;
    std::initializer_list<value_type> source{value_type{7}, value_type{8}, value_type{9}, value_type{10}};

    TestType v;
    v.reserve(8);
    v.emplace_back(11);
    v.emplace_back(22);
    v.emplace_back(33);
    v.emplace_back(44);
    SECTION("insert")
    {
        v.insert(v.begin() , source.begin(), source.end());
        REQUIRE(v == TestType{7, 8, 9, 10, 11, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin()) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 7, 8, 9, 10, 22, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 2) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 7, 8, 9, 10, 33, 44});
    }
    SECTION("insert")
    {
        v.insert(std::next(v.begin(), 3) , source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 7, 8, 9, 10, 44});
    }
    SECTION("insert")
    {
        v.insert(v.end(), source.begin(), source.end());
        REQUIRE(v == TestType{11, 22, 33, 44, 7, 8, 9, 10});
    }
}
