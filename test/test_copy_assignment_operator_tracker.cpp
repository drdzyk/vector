#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "TrackedType.hpp"
#include "Allocator.hpp"

TEMPLATE_PRODUCT_TEST_CASE("copy assignment operator tracker", "[low::vector][std::vector]",
    (low::vector, std::vector), (
        (TrackedType<>, alloc::DynamicEq<TrackedType<>>),
        (TrackedType<>, alloc::DynamicNotEq<TrackedType<>>),
        (TrackedType<>, alloc::DynamicEqPocca<TrackedType<>>)
    )
)
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("copy assignment to empty vector")
    {
        TestType copy;
        copy = source;
        REQUIRE(*tracker == (TypeTracker{.ctor = 2, .copy_ctor = 2}));
    }

    SECTION("copy assignment to not empty vector")
    {
        TestType copy;
        copy.reserve(2);
        copy.emplace_back(tracker);
        copy.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 4}));

        copy = source;
        REQUIRE(*tracker == (TypeTracker{.ctor = 4, .copy_assign = 2}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("copy assignment operator tracker, pocca true, allocators unequal", "[low::vector][std::vector]",
    (low::vector, std::vector), (
        (TrackedType<>, alloc::DynamicNotEqPocca<TrackedType<>>)
    )
)
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("copy assignment to empty vector")
    {
        TestType copy;
        copy = source;
        REQUIRE(*tracker == (TypeTracker{.ctor = 2, .copy_ctor = 2}));
    }

    SECTION("copy assignment to not empty vector")
    {
        TestType copy;
        copy.reserve(2);
        copy.emplace_back(tracker);
        copy.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 4}));

        copy = source;
        REQUIRE(*tracker == (TypeTracker{.ctor = 4, .copy_ctor = 2, .dtor = 2}));
    }
}
