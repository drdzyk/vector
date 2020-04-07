#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "TrackedType.hpp"
#include "Allocator.hpp"

TEMPLATE_PRODUCT_TEST_CASE("insert tracker, enough capacity", "[low::vector][std::vector]",
                          (low::vector, std::vector), (TrackedType<>))
{
    const auto tracker = std::make_shared<TypeTracker>();

    using value_type = typename TestType::value_type;
    std::array source{value_type{tracker}};

    TestType vector;
    vector.reserve(3);
    vector.emplace_back(tracker);
    vector.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

    SECTION("insert")
    {
        vector.insert(std::next(vector.begin()), source.begin(), source.end());
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_assign = 1}));
    }
    SECTION("insert with move iterators")
    {
        vector.insert(std::next(vector.begin()), std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .move_assign = 1}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert tracker, not enough capacity", "[low::vector][std::vector]",
                          (low::vector, std::vector), (TrackedType<>))
{
    const auto tracker = std::make_shared<TypeTracker>();

    using value_type = typename TestType::value_type;
    std::array source{value_type{tracker}};

    TestType vector;
    vector.reserve(2);
    vector.emplace_back(tracker);
    vector.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

    SECTION("insert")
    {
        vector.insert(std::next(vector.begin()), source.begin(), source.end());
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .copy_ctor = 3, .dtor = 2}));
    }
    SECTION("insert with move iterators")
    {
        vector.insert(std::next(vector.begin()), std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_ctor = 2, .dtor = 2}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert tracker, not enough capacity, noexcept move ctor", "[low::vector][std::vector]",
                          (low::vector, std::vector), (TrackedType<NoexceptMoveCtorTag>))
{
    const auto tracker = std::make_shared<TypeTracker>();

    using value_type = typename TestType::value_type;
    std::array source{value_type{tracker}};

    TestType vector;
    vector.reserve(2);
    vector.emplace_back(tracker);
    vector.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

    SECTION("insert")
    {
        vector.insert(std::next(vector.begin()), source.begin(), source.end());
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 2, .copy_ctor = 1, .dtor = 2}));
    }
    SECTION("insert with move iterators")
    {
        vector.insert(std::next(vector.begin()), std::move_iterator{source.begin()}, std::move_iterator{source.end()});
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 3, .dtor = 2}));
    }
}
