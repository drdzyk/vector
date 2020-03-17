#include <gtest/gtest.h>
#include <vector>
#include "vector.hpp"
#include "DynamicInt.hpp"

using Types = testing::Types<int, std::uint32_t, float, double, DynamicInt>;
TYPED_TEST_CASE(VectorTest, Types);

template <typename T>
class VectorTest : public testing::Test
{
protected:
};

TYPED_TEST(VectorTest, vector_move_constructor)
{
    low::vector<TypeParam> source;
    source.emplace_back(7);
    source.emplace_back(8);
    ASSERT_EQ(source[0], 7);
    ASSERT_EQ(source[1], 8);

    auto copy = std::move(source);
    ASSERT_EQ(copy[0], 7);
    ASSERT_EQ(copy[1], 8);
}

TYPED_TEST(VectorTest, vector_move_assignment_operator)
{
    low::vector<TypeParam> source;
    source.emplace_back(7);
    source.emplace_back(8);
    ASSERT_EQ(source[0], 7);
    ASSERT_EQ(source[1], 8);

    low::vector<TypeParam> copy{1, 2, 3};
    copy = std::move(source);
    ASSERT_EQ(copy[0], 7);
    ASSERT_EQ(copy[1], 8);

    copy = std::move(copy); // check self assignment
    ASSERT_EQ(copy[0], 7);
    ASSERT_EQ(copy[1], 8);
}

TYPED_TEST(VectorTest, vector_copy_constructor)
{
  low::vector<TypeParam> source;
  source.emplace_back(7);
  source.emplace_back(8);
  ASSERT_EQ(source[0], 7);
  ASSERT_EQ(source[1], 8);

  auto copy = source;
  ASSERT_EQ(copy[0], 7);
  ASSERT_EQ(copy[1], 8);
}

TYPED_TEST(VectorTest, vector_initializer_list)
{
    {
        std::initializer_list<TypeParam> list;
        low::vector<TypeParam> v{list};
        ASSERT_TRUE(v.empty());
    }
    {
        std::initializer_list<TypeParam> list{7};
        low::vector<TypeParam> v{list};
        ASSERT_EQ(v.size(), 1);
        ASSERT_EQ(v.capacity(), 1);
        ASSERT_EQ(v[0], 7);
    }
    {
        std::initializer_list<TypeParam> list{7, 8};
        low::vector<TypeParam> v{list};
        ASSERT_EQ(v.size(), 2);
        ASSERT_EQ(v.capacity(), 2);
        ASSERT_EQ(v[0], 7);
        ASSERT_EQ(v[1], 8);
    }
    {
        low::vector<TypeParam> v{1, 2, 3};
        ASSERT_EQ(v.size(), 3);
        ASSERT_EQ(v.capacity(), 3);
        ASSERT_EQ(v[0], 1);
        ASSERT_EQ(v[1], 2);
        ASSERT_EQ(v[2], 3);
    }
}
