#include <gtest/gtest.h>
#include "vector.hpp"

struct GlobalTracker
{
    std::size_t ctor{0}, copy_ctor{0}, move_ctor{0}, copy_assign{0}, move_assign{0}, dtor{0};
    std::size_t alloc{0}, dealloc{0};

    bool operator==(const GlobalTracker &r) const noexcept
    {
        return ctor == r.ctor &&
               copy_ctor == r.copy_ctor &&
               move_ctor == r.move_ctor &&
               copy_assign == r.copy_assign &&
               move_assign == r.move_assign &&
               dtor == r.dtor &&
               alloc == r.alloc &&
               dealloc == r.dealloc;
    }

    void clear() noexcept
    {
        ctor = 0;
        copy_ctor = 0;
        move_ctor = 0;
        copy_assign = 0;
        move_assign = 0;
        dtor = 0;
        alloc = 0;
        dealloc = 0;
    }
} global_tracker;

template <typename T>
struct tracked_allocator
{
    using value_type = T;

    T *allocate(std::size_t n)
    {
        ++global_tracker.alloc;
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T *p, std::size_t s)
    {
        ++global_tracker.dealloc;
        std::allocator<T>{}.deallocate(p, s);
    }

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

TEST_F(VectorWithAllocatorTest, track_allocations)
{
    low::vector<int, tracked_allocator<int>> v;
    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1}));
    v.emplace_back(7);
    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1, .alloc = 1, .dealloc = 1})); // TODO: there shouldn't be deallocation
    v.emplace_back(8);
    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1, .alloc = 2, .dealloc = 2}));
    v.resize(0);
    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1, .alloc = 2, .dealloc = 2}));
    v.resize(2);
    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1, .alloc = 2, .dealloc = 2}));
    v.reserve(10);
    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1, .alloc = 3, .dealloc = 3}));
    v.clear(); // don't freed the storage;
    ASSERT_EQ(global_tracker, (GlobalTracker{.ctor = 1, .alloc = 3, .dealloc = 3}));
}
