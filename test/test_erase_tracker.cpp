#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "TrackedType.hpp"
#include "Allocator.hpp"

TEMPLATE_PRODUCT_TEST_CASE("erase tracker", "[low::vector][std::vector]",
                          (low::vector, std::vector), (TrackedType<>))
{
    const auto tracker = std::make_shared<TypeTracker>();

    TestType source;
    source.reserve(3);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 3}));

    SECTION("erase")
    {
        source.erase(source.begin(), std::next(source.begin()));
        REQUIRE(*tracker == (TypeTracker{.ctor = 3, .move_assign = 2, .dtor = 1}));
    }
}
