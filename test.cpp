#include <gtest/gtest.h>
#include <vector>
#include "vector.hpp"

TEST(vector, emplace_back)
{
    low::vector<int> v;
    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v.capacity(), 0);

    v.emplace_back(7);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v.capacity(), 1);
    ASSERT_EQ(v[0], 7);

    v.emplace_back(8);
    ASSERT_EQ(v.size(), 2);
    ASSERT_EQ(v.capacity(), 2);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], 8);

    v.emplace_back(9);
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], 8);
    ASSERT_EQ(v[2], 9);
}

void assertContentEq(const std::vector<int> &src, const low::vector<int> &v2)
{
    ASSERT_EQ(src.size(), v2.size());
    ASSERT_EQ(src.capacity(), v2.capacity());
    for (std::size_t idx{0}; idx < src.size(); ++idx)
    {
        ASSERT_EQ(src[idx], v2[idx]);
    }
}

TEST(vector, emplace_back_regress)
{
    std::vector<int> v1;
    low::vector<int> v2;

    for (std::size_t idx{0}; idx < 100; ++idx)
    {
        v1.emplace_back(idx);
        v2.emplace_back(idx);
        assertContentEq(v1, v2);
    }
}