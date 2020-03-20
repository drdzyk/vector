#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("vector move assignment operator", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType source;
    source.emplace_back(7);
    source.emplace_back(8);
    REQUIRE(source[0] == 7);
    REQUIRE(source[1] == 8);

    TestType copy{1, 2, 3};
    copy = std::move(source);
    REQUIRE(copy[0] == 7);
    REQUIRE(copy[1] == 8);

    using value_type = typename TestType::value_type;
    if constexpr (std::is_same_v<TestType, low::vector<value_type>>)
    {
        // std::vector crashes in this case!
        copy = std::move(copy); // check self assignment
        REQUIRE(copy[0] == 7);
        REQUIRE(copy[1] == 8);
    }
}
