#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "TrackedType.hpp"

TEMPLATE_PRODUCT_TEST_CASE("copy constructor tracker", "[low::vector][std::vector]",
                           (low::vector, std::vector), TrackedType<>)
{
    const auto tracker = std::make_shared<TypeTracker>();
    TestType source;
    source.reserve(2);
    source.emplace_back(tracker);
    source.emplace_back(tracker);
    REQUIRE(*tracker == (TypeTracker{.ctor = 2}));

    SECTION("copy constructor calls copy constructors of stored type")
    {
        auto copy = source;
        REQUIRE(*tracker == (TypeTracker{.ctor = 2, .copy_ctor = 2}));
    }

    SECTION("allocator-extended copy constructor calls copy constructors of stored type")
    {
        typename TestType::allocator_type other;
        TestType copy{source, other};
        REQUIRE(*tracker == (TypeTracker{.ctor = 2, .copy_ctor = 2}));
    }
}

