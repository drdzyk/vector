#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <sstream>
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

std::ostream& operator << (std::ostream& os, const GlobalTracker &t)
{
    os << "ctor: " << t.ctor
       << "; copy_ctor: " << t.copy_ctor
       << "; move_ctor: " << t.move_ctor
       << "; copy_assign: " << t.copy_assign
       << "; move_assign: " << t.move_assign
       << "; dtor: " << t.dtor
       << "; alloc: " << t.alloc
       << "; dealloc: " << t.dealloc;
    return os;
}

template <typename T, bool isEqual = true>
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

    friend bool operator ==(const tracked_allocator &, const tracked_allocator &) noexcept { return isEqual; }
    friend bool operator !=(const tracked_allocator &l, const tracked_allocator &r) noexcept { return !(l == r); }
};

struct Fixture
{
    ~Fixture() noexcept
    {
        global_tracker.clear();
    }
};

TEST_CASE_METHOD(Fixture, "default constructor")
{
    low::vector<int, tracked_allocator<int>> v;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1}));
}


TEST_CASE_METHOD(Fixture, "constructor with allocator")
{
    tracked_allocator<int> alloc;
    low::vector<int, tracked_allocator<int>> v{alloc};

    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .copy_ctor = 1}));
}

TEST_CASE_METHOD(Fixture, "copy constructor")
{
    low::vector<int, tracked_allocator<int>> source;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1}));

    auto copy = source;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .copy_ctor = 1}));
}

TEST_CASE_METHOD(Fixture, "move constructor")
{
    low::vector<int, tracked_allocator<int>> source;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1}));

    auto copy = std::move(source);
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .move_ctor = 1}));
}

TEST_CASE_METHOD(Fixture, "allocator-extended move constructor with equal allocators")
{
    using EqualAlloc = tracked_allocator<int, true>;
    low::vector<int, EqualAlloc> source;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1}));

    EqualAlloc other;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 2}));
    low::vector<int, EqualAlloc> copy{std::move(source), other};
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 3, .copy_ctor = 1, .dtor = 1}));
}

TEST_CASE_METHOD(Fixture, "allocator-extended move constructor with not equal allocators")
{
    using NotEqualAlloc = tracked_allocator<int, false>;
    low::vector<int, NotEqualAlloc> source;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1}));

    NotEqualAlloc other;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 2}));
    low::vector<int, NotEqualAlloc> copy{std::move(source), other};
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 3, .copy_ctor = 1, .dtor = 1}));
}

TEST_CASE_METHOD(Fixture, "move assign operator")
{
    low::vector<int, tracked_allocator<int>> source, copy;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 2}));

    copy = std::move(source);
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 2, .move_assign = 1}));
}

TEST_CASE_METHOD(Fixture, "track allocations")
{
    low::vector<int, tracked_allocator<int>> v;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1}));
    v.emplace_back(7);
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .alloc = 1, .dealloc = 0}));
    v.emplace_back(8);
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .alloc = 2, .dealloc = 1}));
    v.resize(0);
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .alloc = 2, .dealloc = 1}));
    v.resize(2);
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .alloc = 2, .dealloc = 1}));
    v.reserve(10);
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .alloc = 3, .dealloc = 2}));
    v.clear(); // don't freed the storage;
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .alloc = 3, .dealloc = 2}));
    v.shrink_to_fit(); // and now freed it
    REQUIRE(global_tracker == (GlobalTracker{.ctor = 1, .alloc = 3, .dealloc = 3}));
}
