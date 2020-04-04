#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

TEMPLATE_PRODUCT_TEST_CASE("emplace_back", "[low::vector][std::vector]",
    (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    auto &ref1 = v.emplace_back(7);
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 1);
    REQUIRE(v[0] == 7);

    ref1 = 11;
    REQUIRE(v[0] == 11);

    auto &ref2 = v.emplace_back(8);
    REQUIRE(v.size() == 2);
    REQUIRE(v.capacity() == 2);
    REQUIRE(v[0] == 11);
    REQUIRE(v[1] == 8);

    ref2 = 22;
    REQUIRE(v[0] == 11);
    REQUIRE(v[1] == 22);

    auto &ref3 = v.emplace_back(9);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 11);
    REQUIRE(v[1] == 22);
    REQUIRE(v[2] == 9);

    ref3 = 33;
    REQUIRE(v[0] == 11);
    REQUIRE(v[1] == 22);
    REQUIRE(v[2] == 33);
}

void assert_content_eq(const std::vector<int> &src, const low::vector<int> &v2)
{
    REQUIRE(src.size() == v2.size());
    REQUIRE(src.capacity() == v2.capacity());
    for (std::size_t idx{0}; idx < src.size(); ++idx)
    {
        REQUIRE(src[idx] == v2[idx]);
    }
}

TEST_CASE("emplace_back scale", "[low::vector][std::vector]")
{
    std::vector<int> v1;
    low::vector<int> v2;

    for (std::size_t idx{0}; idx < 100; ++idx)
    {
        v1.emplace_back(idx);
        v2.emplace_back(idx);
        assert_content_eq(v1, v2);
    }
}
