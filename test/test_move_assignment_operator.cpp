#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"
#include "Allocator.hpp"

TEMPLATE_PRODUCT_TEST_CASE("move assignment operator", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType source;
    TestType copy;

    SECTION("assign from empty source produce empty vector")
    {
        copy = std::move(source);
        REQUIRE(copy.empty());
    }
    SECTION("assign produce vector with same content")
    {
        source.emplace_back(7);
        source.emplace_back(8);

        copy.emplace_back(9);
        copy.emplace_back(10);
        copy.emplace_back(11);

        copy = std::move(source);
        REQUIRE(copy[0] == 7);
        REQUIRE(copy[1] == 8);
    }
    SECTION("self assignment is ok") // but not for std::vector
    {
        source.emplace_back(7);
        source.emplace_back(8);

        using value_type = typename TestType::value_type;
        if constexpr (std::is_same_v<TestType, low::vector<value_type>>)
        {
            source = std::move(source);
            REQUIRE(source[0] == 7);
            REQUIRE(source[1] == 8);
        }
    }
}

TEMPLATE_PRODUCT_TEST_CASE("move assignment operator different allocators", "[low::vector][std::vector]",
    (low::vector, std::vector), (
        (DynamicInt, alloc::Eq<DynamicInt>),
        (DynamicInt, alloc::EqPocma<DynamicInt>),
        (DynamicInt, alloc::NotEq<DynamicInt>),
        (DynamicInt, alloc::NotEqPocma<DynamicInt>)
    )
)
{
    TestType source;
    TestType copy;

    SECTION("assign from empty to empty")
    {
        copy = std::move(source);
        REQUIRE(copy.empty());
    }
    SECTION("assign from not empty to empty")
    {
        source.emplace_back(7);
        copy = std::move(source);
        REQUIRE(copy.size() == 1);
        REQUIRE(copy[0] == 7);
    }
    SECTION("assign from empty to not empty")
    {
        copy.emplace_back(7);
        copy = std::move(source);
        REQUIRE(copy.empty());
    }
    SECTION("assign produce vector with same content")
    {
        source.emplace_back(7);
        source.emplace_back(8);

        copy.emplace_back(9);
        copy.emplace_back(10);
        copy.emplace_back(11);

        copy = std::move(source);
        REQUIRE(copy[0] == 7);
        REQUIRE(copy[1] == 8);
    }
}
