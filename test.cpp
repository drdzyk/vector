#include <gtest/gtest.h>
#include <vector>
#include "vector.hpp"


class DynamicInt
{
public:
    DynamicInt() = default;
    DynamicInt(int i) : i_(new int(i)) {}
    DynamicInt(const DynamicInt &other) : i_(other.i_ ? new int(*other.i_) : nullptr)
    {
    }
    DynamicInt(DynamicInt &&other): i_(other.i_)
    {
        other.i_ = nullptr;
    }
    DynamicInt& operator=(const DynamicInt &other)
    {
        delete i_;
        i_ = new int(*other.i_);
        return *this;
    }
    DynamicInt& operator=(DynamicInt &&other)
    {
        delete i_;
        i_ = other.i_;
        other.i_ = nullptr;
        return *this;
    }

    ~DynamicInt() { delete i_; }

    friend bool operator==(const DynamicInt &lhs, const DynamicInt &rhs) noexcept
    {
        if (rhs.i_ == nullptr && lhs.i_ == nullptr)
        {
            return true;
        }
        if (rhs.i_ == nullptr || lhs.i_ == nullptr)
        {
            return false;
        }
        return *lhs.i_ == *rhs.i_;
    }
private:
    int *i_{nullptr};
};

using Types = testing::Types<int, std::uint32_t, float, double, DynamicInt>;
TYPED_TEST_CASE(VectorTest, Types);

template <typename T>
class VectorTest : public testing::Test
{
protected:
};

TYPED_TEST(VectorTest, emplace_back)
{
    low::vector<TypeParam> v;
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

TYPED_TEST(VectorTest, iterator)
{
    low::vector<TypeParam> v;
    ASSERT_EQ(v.begin(), v.end());

    v.emplace_back(7);
    ASSERT_NE(v.begin(), v.end());
    ASSERT_EQ(std::distance(v.begin(), v.end()), 1);
    ASSERT_EQ(*v.begin(), 7);

    v.emplace_back(8);
    ASSERT_NE(v.begin(), v.end());
    ASSERT_EQ(std::distance(v.begin(), v.end()), 2);
    ASSERT_EQ(*v.begin(), 7);
    ASSERT_EQ(*std::next(v.begin()), 8);

    *v.begin() = 9;
    ASSERT_EQ(*v.begin(), 9);
    ASSERT_EQ(*std::next(v.begin()), 8);
}


TYPED_TEST(VectorTest, const_iterator)
{
    low::vector<TypeParam> v;
    ASSERT_EQ(v.cbegin(), v.cend());

    v.emplace_back(7);
    ASSERT_NE(v.cbegin(), v.cend());
    ASSERT_EQ(std::distance(v.cbegin(), v.cend()), 1);
    ASSERT_EQ(*v.cbegin(), 7);

    v.emplace_back(8);
    ASSERT_NE(v.cbegin(), v.cend());
    ASSERT_EQ(std::distance(v.cbegin(), v.cend()), 2);
    ASSERT_EQ(*v.cbegin(), 7);
    ASSERT_EQ(*std::next(v.cbegin()), 8);
}

TYPED_TEST(VectorTest, range_based_for)
{
    low::vector<TypeParam> v;
    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);

    auto it = v.begin();
    for (const auto &e : v)
    {
        ASSERT_EQ(e, *it++);
    }
}

template <typename Vector>
static void test_reserve()
{
    Vector v;
    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v.capacity(), 0);

    v.reserve(1);
    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v.capacity(), 1);

    v.emplace_back(77);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v[0], 77);
    ASSERT_EQ(v.capacity(), 1);

    v.reserve(11);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v[0], 77);
    ASSERT_EQ(v.capacity(), 11);

    v.reserve(11);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v[0], 77);
    ASSERT_EQ(v.capacity(), 11);

    v.reserve(7);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v[0], 77);
    ASSERT_EQ(v.capacity(), 11);

    for (std::size_t idx{0}; idx < 10; ++idx)
    {
        v.emplace_back(idx);
        ASSERT_EQ(v.size(), idx + 2);
        ASSERT_EQ(v.capacity(), 11);
    }
}

TYPED_TEST(VectorTest, reserve)
{
    test_reserve<low::vector<TypeParam>>();
}

TYPED_TEST(VectorTest, reserve_std_vector)
{
    test_reserve<std::vector<TypeParam>>();
}

template <typename Vector>
static void test_resize()
{
    using value_type =  typename Vector::value_type;

    Vector v;
    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);

    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], 8);
    ASSERT_EQ(v[2], 9);

    v.resize(1);
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);

    v.resize(3);
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], value_type{});
    ASSERT_EQ(v[2], value_type{});

    v.resize(4);
    ASSERT_EQ(v.size(), 4);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], value_type{});
    ASSERT_EQ(v[2], value_type{});
    ASSERT_EQ(v[3], value_type{});

    v.resize(7);
    ASSERT_EQ(v.size(), 7);
    ASSERT_EQ(v.capacity(), 8);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], value_type{});
    ASSERT_EQ(v[2], value_type{});
    ASSERT_EQ(v[3], value_type{});
    ASSERT_EQ(v[4], value_type{});
    ASSERT_EQ(v[5], value_type{});
    ASSERT_EQ(v[6], value_type{});
}

template <typename Vector>
static void test_valued_resize()
{
    using value_type =  typename Vector::value_type;

    Vector v;
    v.emplace_back(7);
    v.emplace_back(8);
    v.emplace_back(9);

    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], 8);
    ASSERT_EQ(v[2], 9);

    v.resize(1, value_type{777});
    ASSERT_EQ(v.size(), 1);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);

    v.resize(3, value_type{666});
    ASSERT_EQ(v.size(), 3);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], value_type{666});
    ASSERT_EQ(v[2], value_type{666});

    v.resize(4);
    ASSERT_EQ(v.size(), 4);
    ASSERT_EQ(v.capacity(), 4);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], value_type{666});
    ASSERT_EQ(v[2], value_type{666});
    ASSERT_EQ(v[3], value_type{});

    v.resize(7, value_type{999});
    ASSERT_EQ(v.size(), 7);
    ASSERT_EQ(v.capacity(), 8);
    ASSERT_EQ(v[0], 7);
    ASSERT_EQ(v[1], value_type{666});
    ASSERT_EQ(v[2], value_type{666});
    ASSERT_EQ(v[3], value_type{});
    ASSERT_EQ(v[4], value_type{999});
    ASSERT_EQ(v[5], value_type{999});
    ASSERT_EQ(v[6], value_type{999});
}

TYPED_TEST(VectorTest, resize)
{
    test_resize<low::vector<TypeParam>>();
    test_valued_resize<low::vector<TypeParam>>();
}

TYPED_TEST(VectorTest, resize_std_vector)
{
    test_resize<std::vector<TypeParam>>();
    test_valued_resize<std::vector<TypeParam>>();
}

template <typename Vector>
void test_resize_scale()
{
    using value_type =  typename Vector::value_type;
    Vector v;

    v.resize(8);
    ASSERT_EQ(v.size(), 8);
    ASSERT_EQ(v.capacity(), 8);

    v.resize(0);
    ASSERT_EQ(v.size(), 0);
    ASSERT_EQ(v.capacity(), 8);

    v.resize(7);
    ASSERT_EQ(v.size(), 7);
    ASSERT_EQ(v.capacity(), 8);

    v.resize(11);
    ASSERT_EQ(v.size(), 11);
    ASSERT_EQ(v.capacity(), 14);

    v.resize(23);
    ASSERT_EQ(v.size(), 23);
    ASSERT_EQ(v.capacity(), 23);
}

TYPED_TEST(VectorTest, resize_scale)
{
    test_resize_scale<low::vector<TypeParam>>();
}

TYPED_TEST(VectorTest, resize_scale_std_vector)
{
    test_resize_scale<std::vector<TypeParam>>();
}

template <typename T>
void assert_content_eq(const std::vector<T> &src, const low::vector<T> &v2)
{
    ASSERT_EQ(src.size(), v2.size());
    ASSERT_EQ(src.capacity(), v2.capacity());
    for (std::size_t idx{0}; idx < src.size(); ++idx)
    {
        ASSERT_EQ(src[idx], v2[idx]);
    }
}

TYPED_TEST(VectorTest, emplace_back_regress)
{
    std::vector<TypeParam> v1;
    low::vector<TypeParam> v2;

    for (std::size_t idx{0}; idx < 100; ++idx)
    {
        v1.emplace_back(idx);
        v2.emplace_back(idx);
        assert_content_eq(v1, v2);
    }
}

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

    low::vector<TypeParam> copy;
    copy = std::move(source);
    ASSERT_EQ(copy[0], 7);
    ASSERT_EQ(copy[1], 8);

    copy = std::move(copy); // check self assignment
    ASSERT_EQ(copy[0], 7);
    ASSERT_EQ(copy[1], 8);
}
