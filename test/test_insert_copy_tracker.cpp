#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "TrackedType.hpp"
#include "Allocator.hpp"

TEMPLATE_PRODUCT_TEST_CASE("insert copy tracker, enough capacity", "[low::vector][std::vector]",
                          (low::vector, std::vector), (TrackedType<>))
{
    using value_type = typename TestType::value_type;
    using allocator_type = typename TestType::allocator_type;

    const auto tracker = std::make_shared<TypeTracker>();
    value_type value{tracker};

    TestType vector;
    vector.reserve(3);
    vector.emplace_back(tracker);
    vector.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

    SECTION("insert with count")
    {
        vector.insert(std::next(vector.begin()), 1, value);
        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            // std::vector produce 1 additional tmp object
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_ctor = 1, .copy_assign = 1, .dtor = 1}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_assign = 1}));
    }
    SECTION("insert with count ignore move")
    {
        vector.insert(std::next(vector.begin()), 1, std::move(value));

        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            // std::vector produce 1 additional tmp object
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_ctor = 1, .copy_assign = 1, .dtor = 1}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_assign = 1}));

    }
    SECTION("insert without count")
    {
        vector.insert(std::next(vector.begin()), value);

        if constexpr (std::is_same_v<TestType, std::vector<value_type, allocator_type>>)
            // std::vector produce 1 additional tmp object
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_ctor = 1, .move_assign = 1, .dtor = 1}));
        else
            REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_assign = 1}));
    }
    SECTION("insert without count + move")
    {
        vector.insert(std::next(vector.begin()), std::move(value));
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .move_assign = 1}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert copy tracker, not enough capacity", "[low::vector][std::vector]",
                          (low::vector, std::vector), (TrackedType<>))
{
    const auto tracker = std::make_shared<TypeTracker>();

    using value_type = typename TestType::value_type;
    value_type value{tracker};

    TestType vector;
    vector.reserve(2);
    vector.emplace_back(tracker);
    vector.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

    SECTION("insert")
    {
        vector.insert(std::next(vector.begin()), value);
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .copy_ctor = 3, .dtor = 2}));
    }
    SECTION("insert move")
    {
        vector.insert(std::next(vector.begin()), std::move(value));
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 1, .copy_ctor = 2, .dtor = 2}));
    }
}

TEMPLATE_PRODUCT_TEST_CASE("insert copy tracker, not enough capacity, noexcept move ctor", "[low::vector][std::vector]",
                          (low::vector, std::vector), (TrackedType<NoexceptMoveCtorTag>))
{
    const auto tracker = std::make_shared<TypeTracker>();

    using value_type = typename TestType::value_type;
    value_type value{tracker};

    TestType vector;
    vector.reserve(2);
    vector.emplace_back(tracker);
    vector.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

    SECTION("insert")
    {
        vector.insert(std::next(vector.begin()), value);
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 2, .copy_ctor = 1, .dtor = 2}));
    }
    SECTION("insert move")
    {
        vector.insert(std::next(vector.begin()), std::move(value));
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_ctor = 3, .dtor = 2}));
    }
}
