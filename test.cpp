#include <gtest/gtest.h>
#include <vector>
#include "vector.hpp"

TEST(vector, emplace_back)
{
    low::vector<int> v;
    v.emplace_back(7);
    ASSERT_EQ(v[0], 7);

    v.emplace_back(8);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], 8);

    v.emplace_back(9);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], 8);
    ASSERT_EQ(v[2], 9);
}

void assertContentEq(const std::vector<int> &src, const low::vector<int> &v2)
{
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