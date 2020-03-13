#include <gtest/gtest.h>
#include "vector.hpp"

struct GlobalTracker
{
    std::size_t ctor{0}, copy_ctor{0}, move_ctor{0}, copy_assign{0}, move_assign{0}, dtor{0};

    bool operator==(const GlobalTracker &r) const noexcept
    {
        return ctor == r.ctor &&
               copy_ctor == r.copy_ctor &&
               move_ctor == r.move_ctor &&
               copy_assign == r.copy_assign &&
               move_assign == r.move_assign &&
               dtor == r.dtor;
    }

    void clear() noexcept
    {
        ctor = 0;
        copy_ctor = 0;
        move_ctor = 0;
        copy_assign = 0;
        move_assign = 0;
        dtor = 0;
    }
} global_tracker;

template <typename T>
struct tracked_allocator : std::allocator<T>
{
    tracked_allocator() noexcept
    {
        ++global_tracker.ctor;
    }
    tracked_allocator(const tracked_allocator &other) noexcept
    {
        ++global_tracker.copy_ctor;
    }
    tracked_allocator(tracked_allocator &&other) noexcept
    {
        ++global_tracker.move_ctor;
    }
    tracked_allocator& operator=(const tracked_allocator &other) noexcept
    {
        ++global_tracker.copy_assign;
        return *this;
    }
    tracked_allocator& operator=(tracked_allocator &&other) noexcept
    {
        ++global_tracker.move_assign;
        return *this;
    }
    ~tracked_allocator() noexcept
    {
        ++global_tracker.dtor;
    }
};

class VectorWithAllocatorTest : public testing::Test
{
protected:
    void SetUp() override { global_tracker.clear(); }
};

TEST_F(VectorWithAllocatorTest, default_ctor)
{
    low::vector<int, tracked_allocator<int>> v;
    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1}));
}

TEST_F(VectorWithAllocatorTest, ctor_with_allocator)
{
    tracked_allocator<int> alloc;
    low::vector<int, tracked_allocator<int>> v{alloc};

    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1, .copy_ctor = 1}));
}
