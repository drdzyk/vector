#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "TrackedType.hpp"

TEMPLATE_PRODUCT_TEST_CASE("reserve on throwing move ctor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (TrackedType<>))
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType v;

    v.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 1}));

    v.reserve(2);
    REQUIRE(*tracker == (TypeTracker{.ctor = 1, .copy_ctor = 1, .dtor = 1}));
}

TEMPLATE_PRODUCT_TEST_CASE("reserve on noexcept move ctor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (TrackedType<NoexceptMoveCtorTag>))
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType v;

    v.emplace_back(tracker);
    REQUIRE(*tracker ==(TypeTracker{.ctor = 1}));

    v.reserve(2);
    REQUIRE(*tracker == (TypeTracker{.ctor = 1, .move_ctor = 1, .dtor = 1}));
}

TEMPLATE_PRODUCT_TEST_CASE("resize on throwing move ctor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (TrackedType<>))
{
    const auto tracker = std::make_shared<TypeTracker>();
    low::vector<TrackedType<>> v;

    v.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 1}));

    v.resize(2, tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2, .copy_ctor = 2, .dtor = 2}));
}

TEMPLATE_PRODUCT_TEST_CASE("resize on noexcept move ctor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (TrackedType<NoexceptMoveCtorTag>))
{
    const auto tracker = std::make_shared<TypeTracker>();
    low::vector<TrackedType<NoexceptMoveCtorTag>> v;

    v.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 1}));

    v.resize(2, tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2, .copy_ctor = 1, .move_ctor = 1, .dtor = 2}));
}
