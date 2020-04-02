#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "Allocator.hpp"
#include "TrackedType.hpp"

TEMPLATE_PRODUCT_TEST_CASE("move constructor tracker", "[low::vector][std::vector]",
                           (low::vector, std::vector), TrackedType<>)
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("move constructor just steal pointers")
    {
        auto copy = std::move(source);
        REQUIRE(*tracker == (TypeTracker{.ctor = 2}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("allocator-extended move constructor tracker, equal allocator", "",
    (low::vector, std::vector), (
        (TrackedType<>, alloc::StaticEq<TrackedType<>>),
        (TrackedType<>, alloc::DynamicEq<TrackedType<>>)
    )
)
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("move constructor which accept equal allocator steal pointers too")
    {
        typename TestType::allocator_type other;
        TestType copy{std::move(source), other};
        REQUIRE(*tracker == (TypeTracker{.ctor = 2}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("allocator-extended move constructor tracker with wrong allocator", "",
    (low::vector), ((TrackedType<>, alloc::WrongEq<TrackedType<>>))) // std::vector son't use is_always_equal here
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("move constructor which accept equal allocator steal pointers too")
    {
        typename TestType::allocator_type other;
        TestType copy{std::move(source), other};
        REQUIRE(*tracker == (TypeTracker{.ctor = 2}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("allocator-extended move constructor tracker, not equal allocator", "",
                           (low::vector, std::vector), ((TrackedType<>, alloc::DynamicNotEq<TrackedType<>>)))
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("move constructor which accept not equal allocator move stored objects")
    {
        using value_type = typename TestType::value_type;
        using allocator_type = typename TestType::allocator_type;

        allocator_type other;
        TestType copy{std::move(source), other};
        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            REQUIRE(*tracker == (TypeTracker{.ctor = 2, .move_ctor = 2, .dtor = 2}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 2, .move_ctor = 2}));
    }
}
