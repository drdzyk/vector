#define CATCH_CONFIG_DISABLE_MATCHERS
#define CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_COUNTER
#include <catch2/catch.hpp>
#include <vector>
#include "vector.hpp"

struct Tracker
{
    std::size_t ctor{0}, copy_ctor{0}, move_ctor{0}, copy_assign{0}, move_assign{0}, dtor{0};

    bool operator==(const Tracker&r) const noexcept
    {
        return ctor == r.ctor &&
            copy_ctor == r.copy_ctor &&
            move_ctor == r.move_ctor &&
            copy_assign == r.copy_assign &&
            move_assign == r.move_assign &&
            dtor == r.dtor;
    }
};

struct NoexceptCtorTag{};
struct NoexceptCopyCtorTag{};
struct NoexceptMoveCtorTag{};
struct NoexceptCopyAssignTag{};
struct NoexceptMoveAssignTag{};

template <typename ...Tags>
class A
{
public:
    A(const std::shared_ptr<Tracker> &tracker) noexcept((std::is_same_v<NoexceptCtorTag, Tags> || ...))
      : tracker_(tracker)
    {
        ++tracker_->ctor;
    }
    A(const A &other) noexcept((std::is_same_v<NoexceptCopyCtorTag, Tags> || ...))
      : tracker_(other.tracker_)
    {
        ++tracker_->copy_ctor;
    }
    A(A &&other) noexcept((std::is_same_v<NoexceptMoveCtorTag, Tags> || ...))
      : tracker_(other.tracker_)
    {
        ++tracker_->move_ctor;
    }
    A& operator=(const A &other) noexcept((std::is_same_v<NoexceptCopyAssignTag, Tags> || ...))
    {
        ++tracker_->copy_assign;
        return *this;
    }
    A& operator=(A &&other) noexcept((std::is_same_v<NoexceptMoveAssignTag, Tags> || ...))
    {
        ++tracker_->move_assign;
        return *this;
    }
    // should be always noexcept, otherwise std::is_nothrow_move_constructible_v will return false,
    // whatever move ctor noexcept specifier
    ~A() noexcept
    {
        ++tracker_->dtor;
    }

private:
    const std::shared_ptr<Tracker> tracker_;
};

TEMPLATE_PRODUCT_TEST_CASE("reserve on throwing move ctor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (A<>))
{
    const auto tracker = std::make_shared<Tracker>();
    TestType v;

    v.emplace_back(tracker);
    REQUIRE(*tracker == (Tracker{.ctor = 1}));

    v.reserve(2);
    REQUIRE(*tracker == (Tracker{.ctor = 1, .copy_ctor = 1, .dtor = 1}));
}

TEMPLATE_PRODUCT_TEST_CASE("reserve on noexcept move ctor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (A<NoexceptMoveCtorTag>))
{
    const auto tracker = std::make_shared<Tracker>();
    TestType v;

    v.emplace_back(tracker);
    REQUIRE(*tracker ==(Tracker{.ctor = 1}));

    v.reserve(2);
    REQUIRE(*tracker == (Tracker{.ctor = 1, .move_ctor = 1, .dtor = 1}));
}

TEMPLATE_PRODUCT_TEST_CASE("resize on throwing move ctor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (A<>))
{
    const auto tracker = std::make_shared<Tracker>();
    low::vector<A<>> v;

    v.emplace_back(tracker);
    REQUIRE(*tracker == (Tracker{.ctor = 1}));

    v.resize(2, tracker);
    REQUIRE(*tracker == (Tracker{.ctor = 2, .copy_ctor = 2, .dtor = 2}));
}

TEMPLATE_PRODUCT_TEST_CASE("resize on noexcept move ctor", "[low::vector][std::vector]",
                           (low::vector, std::vector), (A<NoexceptMoveCtorTag>))
{
    const auto tracker = std::make_shared<Tracker>();
    low::vector<A<NoexceptMoveCtorTag>> v;

    v.emplace_back(tracker);
    REQUIRE(*tracker == (Tracker{.ctor = 1}));

    v.resize(2, tracker);
    REQUIRE(*tracker == (Tracker{.ctor = 2, .copy_ctor = 1, .move_ctor = 1, .dtor = 2}));
}
