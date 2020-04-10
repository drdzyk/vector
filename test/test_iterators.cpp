#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("begin, end", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.begin() == v.end());

    v.emplace_back(7);
    REQUIRE(v.begin() != v.end());
    REQUIRE(std::distance(v.begin(), v.end()) == 1);
    REQUIRE(*v.begin() == 7);

    v.emplace_back(8);
    REQUIRE(v.begin() != v.end());
    REQUIRE(std::distance(v.begin(), v.end()) == 2);
    REQUIRE(*v.begin() == 7);
    REQUIRE(*std::next(v.begin()) == 8);

    *v.begin() = 9;
    REQUIRE(*v.begin() == 9);
    REQUIRE(*std::next(v.begin()) == 8);
}

TEMPLATE_PRODUCT_TEST_CASE("begin, end const overloads", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    const TestType v{7, 8, 9};

    REQUIRE(v.begin() != v.end());
    REQUIRE(std::distance(v.begin(), v.end()) == 3);
    REQUIRE(*v.begin() == 7);
    REQUIRE(*std::next(v.begin()) == 8);
    REQUIRE(*std::next(v.begin(), 2) == 9);
}

TEMPLATE_PRODUCT_TEST_CASE("cbegin, cend", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.cbegin() == v.cend());

    v.emplace_back(7);
    REQUIRE(v.cbegin() != v.cend());
    REQUIRE(std::distance(v.cbegin(), v.cend()) == 1);
    REQUIRE(*v.cbegin() == 7);

    v.emplace_back(8);
    REQUIRE(v.cbegin() != v.cend());
    REQUIRE(std::distance(v.cbegin(), v.cend()) == 2);
    REQUIRE(*v.cbegin() == 7);
    REQUIRE(*std::next(v.cbegin()) == 8);
}

TEMPLATE_PRODUCT_TEST_CASE("range based for", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v{7, 8, 9};

    auto it = v.begin();
    for (const auto &e : v)
    {
        REQUIRE(e == *it++);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("rbegin, rend", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.rbegin() == v.rend());

    v.emplace_back(7);
    REQUIRE(v.rbegin() != v.rend());
    REQUIRE(std::distance(v.rbegin(), v.rend()) == 1);
    REQUIRE(*v.rbegin() == 7);

    v.emplace_back(8);
    REQUIRE(v.rbegin() != v.rend());
    REQUIRE(std::distance(v.rbegin(), v.rend()) == 2);
    REQUIRE(*v.rbegin() == 8);
    REQUIRE(*std::next(v.rbegin()) == 7);

    *v.rbegin() = 9;
    REQUIRE(*v.rbegin() == 9);
    REQUIRE(*std::next(v.rbegin()) == 7);
}


TEMPLATE_PRODUCT_TEST_CASE("rbegin, rend const overloads", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    const TestType v{7, 8, 9};

    REQUIRE(v.rbegin() != v.rend());
    REQUIRE(std::distance(v.rbegin(), v.rend()) == 3);
    REQUIRE(*v.rbegin() == 9);
    REQUIRE(*std::next(v.rbegin()) == 8);
    REQUIRE(*std::next(v.rbegin(), 2) == 7);
}


TEMPLATE_PRODUCT_TEST_CASE("crbegin, crend", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.crbegin() == v.crend());

    v.emplace_back(7);
    REQUIRE(v.crbegin() != v.crend());
    REQUIRE(std::distance(v.crbegin(), v.crend()) == 1);
    REQUIRE(*v.crbegin() == 7);

    v.emplace_back(8);
    REQUIRE(v.crbegin() != v.crend());
    REQUIRE(std::distance(v.crbegin(), v.crend()) == 2);
    REQUIRE(*v.crbegin() == 8);
    REQUIRE(*std::next(v.crbegin()) == 7);
}
