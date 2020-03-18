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

    v.emplace_back(7);
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 1);
    REQUIRE(v[0] == 7);

    v.emplace_back(8);
    REQUIRE(v.size() == 2);
    REQUIRE(v.capacity() == 2);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);

    v.emplace_back(9);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);
    REQUIRE(v[2] == 9);
}

TEMPLATE_PRODUCT_TEST_CASE("begin, end", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.begin() == v.end());

    v.emplace_back(7);
    REQUIRE(v.begin() != v.end());
    REQUIRE(std::distance(v.begin(), v.end()) == 1);
    REQUIRE(*v.begin() == 7);

    v.emplace_back(8);
    REQUIRE(v.begin() != v.end());
    REQUIRE(std::distance(v.begin(), v.end()) == 2);
    REQUIRE(*v.begin() == 7);
    REQUIRE(*std::next(v.begin()) == 8);

    *v.begin() = 9;
    REQUIRE(*v.begin() == 9);
    REQUIRE(*std::next(v.begin()) == 8);
}

TEMPLATE_PRODUCT_TEST_CASE("begin, end const overloads", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    const TestType v{7, 8, 9};

    REQUIRE(v.cbegin() != v.cend());
    REQUIRE(std::distance(v.cbegin(), v.cend()) == 3);
    REQUIRE(*v.begin() == 7);
    REQUIRE(*std::next(v.begin()) == 8);
    REQUIRE(*std::next(v.begin(), 2) == 9);
}

TEMPLATE_PRODUCT_TEST_CASE("cbegin, cend", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.cbegin() == v.cend());

    v.emplace_back(7);
    REQUIRE(v.cbegin() != v.cend());
    REQUIRE(std::distance(v.cbegin(), v.cend()) == 1);
    REQUIRE(*v.cbegin() == 7);

    v.emplace_back(8);
    REQUIRE(v.cbegin() != v.cend());
    REQUIRE(std::distance(v.cbegin(), v.cend()) == 2);
    REQUIRE(*v.cbegin() == 7);
    REQUIRE(*std::next(v.cbegin()) == 8);
}

TEMPLATE_PRODUCT_TEST_CASE("range based for", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v{7, 8, 9};

    auto it = v.begin();
    for (const auto &e : v)
    {
        REQUIRE(e == *it++);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("reserve", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    v.reserve(1);
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 1);

    v.emplace_back(77);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 77);
    REQUIRE(v.capacity() == 1);

    v.reserve(11);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 77);
    REQUIRE(v.capacity() == 11);

    v.reserve(11);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 77);
    REQUIRE(v.capacity() == 11);

    v.reserve(7);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 77);
    REQUIRE(v.capacity() == 11);

    for (std::size_t idx{0}; idx < 10; ++idx)
    {
        v.emplace_back(idx);
        REQUIRE(v.size() == idx + 2);
        REQUIRE(v.capacity() == 11);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("resize", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    using value_type =  typename TestType::value_type;

    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);

    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);
    REQUIRE(v[2] == 9);

    v.resize(1);
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);

    v.resize(3);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{});
    REQUIRE(v[2] == value_type{});

    v.resize(4);
    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{});
    REQUIRE(v[2] == value_type{});
    REQUIRE(v[3] == value_type{});

    v.resize(7);
    REQUIRE(v.size() == 7);
    REQUIRE(v.capacity() == 8);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{});
    REQUIRE(v[2] == value_type{});
    REQUIRE(v[3] == value_type{});
    REQUIRE(v[4] == value_type{});
    REQUIRE(v[5] == value_type{});
    REQUIRE(v[6] == value_type{});
}

TEMPLATE_PRODUCT_TEST_CASE("resize with value", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    using value_type =  typename TestType::value_type;

    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);

    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);
    REQUIRE(v[2] == 9);

    v.resize(1, value_type{777});
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);

    v.resize(3, value_type{666});
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{666});
    REQUIRE(v[2] == value_type{666});

    v.resize(4);
    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{666});
    REQUIRE(v[2] == value_type{666});
    REQUIRE(v[3] == value_type{});

    v.resize(7, value_type{999});
    REQUIRE(v.size() == 7);
    REQUIRE(v.capacity() == 8);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == value_type{666});
    REQUIRE(v[2] == value_type{666});
    REQUIRE(v[3] == value_type{});
    REQUIRE(v[4] == value_type{999});
    REQUIRE(v[5] == value_type{999});
    REQUIRE(v[6] == value_type{999});
}

TEMPLATE_PRODUCT_TEST_CASE("resize scale", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    using value_type =  typename TestType::value_type;

    v.resize(8);
    REQUIRE(v.size() == 8);
    REQUIRE(v.capacity() == 8);

    v.resize(0);
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 8);

    v.resize(7);
    REQUIRE(v.size() == 7);
    REQUIRE(v.capacity() == 8);

    v.resize(11);
    REQUIRE(v.size() == 11);
    REQUIRE(v.capacity() == 14);

    v.resize(23);
    REQUIRE(v.size() == 23);
    REQUIRE(v.capacity() == 23);
}

TEMPLATE_PRODUCT_TEST_CASE("clear", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;

    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);

    v.clear();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 4);

    v.clear(); // double clear() ok
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 4);

    v.emplace_back(7);
    v.emplace_back(8);
    REQUIRE(v.size() == 2);
    REQUIRE(v.capacity() == 4);
    REQUIRE(v[0] == 7);
    REQUIRE(v[1] == 8);
}

TEMPLATE_PRODUCT_TEST_CASE("shrink_to_fit", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;

    v.shrink_to_fit();
    v.shrink_to_fit(); // double shrink_to_fit() is ok

    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);

    v.shrink_to_fit();
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 3);

    v.shrink_to_fit(); // nothing changed
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 3);

    v.clear();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 3);

    v.shrink_to_fit();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    v.shrink_to_fit(); // double shrink_to_fit() is ok
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}

TEMPLATE_PRODUCT_TEST_CASE("empty, size", "[low::vector][std::vector]",
                           (low::vector, std::vector), (int, double, DynamicInt))
{
    TestType v;
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);

    v.emplace_back(7);
    REQUIRE_FALSE(v.empty());
    REQUIRE(v.size() == 1);

    v.clear();
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
}

template <typename T>
void assert_content_eq(const std::vector<T> &src, const low::vector<T> &v2)
{
    REQUIRE(src.size() == v2.size());
    REQUIRE(src.capacity() == v2.capacity());
    for (std::size_t idx{0}; idx < src.size(); ++idx)
    {
        REQUIRE(src[idx] == v2[idx]);
    }
}

TEMPLATE_TEST_CASE("emplace_back scale", "[low::vector][std::vector]", int, double, DynamicInt)
{
    std::vector<TestType> v1;
    low::vector<TestType> v2;

    for (std::size_t idx{0}; idx < 100; ++idx)
    {
        v1.emplace_back(idx);
        v2.emplace_back(idx);
        assert_content_eq(v1, v2);
    }
}
