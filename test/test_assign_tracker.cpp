#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include <array>
#include "vector.hpp"
#include "TrackedType.hpp"

TEMPLATE_PRODUCT_TEST_CASE("assign tracker with move iterators", "[low::vector][std::vector]",
                           (low::vector, std::vector), TrackedType<>)
{
    const auto tracker = std::make_shared<TypeTracker>();
    std::array<TrackedType<>, 2> source{tracker, tracker};
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("move to empty vector")
    {
        TestType v;
        v.assign(std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 2, .move_ctor = 2}));
    }
    SECTION("move to not empty vector with not enough capacity")
    {
        TestType v;
        v.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

        v.assign(std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 2, .dtor = 1}));
    }
    SECTION("move to not empty vector with enough capacity")
    {
        TestType v;
        v.reserve(2);
        v.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

        v.assign(std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .move_assign = 1}));
    }
    SECTION("move to not empty vector with enough capacity")
    {
        TestType v;
        v.reserve(2);
        v.emplace_back(tracker);
        v.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 4}));

        v.assign(std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 4, .move_assign = 2}));
    }
    SECTION("move to not empty vector with enough capacity")
    {
        TestType v;
        v.reserve(3);
        v.emplace_back(tracker);
        v.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 4}));

        v.assign(std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 4, .move_assign = 2}));
    }
    SECTION("move to not empty vector with enough capacity")
    {
        TestType v;
        v.reserve(3);
        v.emplace_back(tracker);
        v.emplace_back(tracker);
        v.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 5}));

        v.assign(std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 5, .move_assign = 2, .dtor = 1}));
    }
    SECTION("move to not empty vector with enough capacity")
    {
        TestType v;
        v.reserve(5);
        v.emplace_back(tracker);
        v.emplace_back(tracker);
        v.emplace_back(tracker);
        v.emplace_back(tracker);
        v.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 7}));

        v.assign(std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 7, .move_assign = 2, .dtor = 3}));
    }
}
