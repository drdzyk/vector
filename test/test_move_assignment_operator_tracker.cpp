#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "TrackedType.hpp"
#include "Allocator.hpp"

TEMPLATE_PRODUCT_TEST_CASE("move assignment operator tracker", "[low::vector][std::vector]",
    (low::vector, std::vector), (
        (TrackedType<>, alloc::StaticEq<TrackedType<>>),
        (TrackedType<>, alloc::StaticEqPocma<TrackedType<>>),
        (TrackedType<>, alloc::DynamicEqPocma<TrackedType<>>),
        (TrackedType<>, alloc::DynamicNotEqPocma<TrackedType<>>),
        (TrackedType<>, alloc::DynamicEq<TrackedType<>>),
        (TrackedType<>, alloc::WrongEq<TrackedType<>>)
    )
)
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("move assignment to empty vector")
    {
        TestType copy;
        copy = std::move(source);
        REQUIRE(*tracker == (TypeTracker{.ctor = 2}));
    }

    SECTION("move assignment to not empty vector")
    {
        TestType copy;
        copy.reserve(2);
        copy.emplace_back(tracker);
        copy.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 4}));

        copy = std::move(source);
        REQUIRE(*tracker == (TypeTracker{.ctor = 4, .dtor = 2}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("move assignment operator tracker, pocma false, allocators unequal", "[low::vector][std::vector]",
    (low::vector, std::vector), (
        (TrackedType<>, alloc::DynamicNotEq<TrackedType<>>)
    )
)
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    using value_type = typename TestType::value_type;
    using allocator_type = typename TestType::allocator_type;

    SECTION("move assignment to empty vector")
    {
        TestType copy;
        copy = std::move(source);

        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            REQUIRE(*tracker == (TypeTracker{.ctor = 2, .copy_ctor = 2, .dtor = 2}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 2, .copy_ctor = 2}));
    }

    SECTION("move assignment to not empty vector")
    {
        TestType copy;
        copy.reserve(2);
        copy.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

        copy = std::move(source);

        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .copy_ctor = 1, .copy_assign = 1, .dtor = 2}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .copy_ctor = 1, .copy_assign = 1}));
    }

    SECTION("move assignment to not empty vector")
    {
        TestType copy;
        copy.reserve(2);
        copy.emplace_back(tracker);
        copy.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 4}));

        copy = std::move(source);

        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            REQUIRE(*tracker == (TypeTracker{.ctor = 4, .copy_assign = 2, .dtor = 2}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 4, .copy_assign = 2}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("move assignment operator tracker, pocma false, allocators unequal", "noexcept TrackedType",
    (low::vector, std::vector), (
        (TrackedType<NoexceptMoveCtorTag>, alloc::DynamicNotEq<TrackedType<NoexceptMoveCtorTag>>)
    )
)
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    using value_type = typename TestType::value_type;
    using allocator_type = typename TestType::allocator_type;

    SECTION("move assignment to empty vector")
    {
        TestType copy;
        copy = std::move(source);

        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            REQUIRE(*tracker == (TypeTracker{.ctor = 2, .move_ctor = 2, .dtor = 2}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 2, .move_ctor = 2}));
    }

    SECTION("move assignment to not empty vector")
    {
        TestType copy;
        copy.reserve(2);
        copy.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

        copy = std::move(source);

        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .move_assign = 1, .dtor = 2}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .move_assign = 1}));
    }

    SECTION("move assignment to not empty vector")
    {
        TestType copy;
        copy.reserve(2);
        copy.emplace_back(tracker);
        copy.emplace_back(tracker);
        REQUIRE(*tracker == (TypeTracker{.ctor = 4}));

        copy = std::move(source);

        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            REQUIRE(*tracker == (TypeTracker{.ctor = 4, .move_assign = 2, .dtor = 2}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 4, .move_assign = 2}));
    }
}
