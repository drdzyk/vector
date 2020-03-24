#pragma once
#include <memory>
#include <type_traits>

struct TypeTracker
{
    std::size_t ctor{0}, copy_ctor{0}, move_ctor{0}, copy_assign{0}, move_assign{0}, dtor{0};

    bool operator==(const TypeTracker&r) const noexcept
    {
        return ctor == r.ctor &&
            copy_ctor == r.copy_ctor &&
            move_ctor == r.move_ctor &&
            copy_assign == r.copy_assign &&
            move_assign == r.move_assign &&
            dtor == r.dtor;
    }
};

// make Catch2 reports about assertion failure more verbose
inline std::ostream& operator << (std::ostream& os, const TypeTracker &t)
{
    os << "ctor: " << t.ctor
       << "; copy_ctor: " << t.copy_ctor
       << "; move_ctor: " << t.move_ctor
       << "; copy_assign: " << t.copy_assign
       << "; move_assign: " << t.move_assign
       << "; dtor: " << t.dtor;
    return os;
}

struct NoexceptCtorTag{};
struct NoexceptCopyCtorTag{};
struct NoexceptMoveCtorTag{};
struct NoexceptCopyAssignTag{};
struct NoexceptMoveAssignTag{};

template <typename ...Tags>
class TrackedType
{
public:
    TrackedType(const std::shared_ptr<TypeTracker> &tracker) noexcept((std::is_same_v<NoexceptCtorTag, Tags> || ...))
      : tracker_(tracker)
    {
        ++tracker_->ctor;
    }
    TrackedType(const TrackedType &other) noexcept((std::is_same_v<NoexceptCopyCtorTag, Tags> || ...))
      : tracker_(other.tracker_)
    {
        ++tracker_->copy_ctor;
    }
    TrackedType(TrackedType &&other) noexcept((std::is_same_v<NoexceptMoveCtorTag, Tags> || ...))
      : tracker_(other.tracker_)
    {
        ++tracker_->move_ctor;
    }
    TrackedType& operator=(const TrackedType &other) noexcept((std::is_same_v<NoexceptCopyAssignTag, Tags> || ...))
    {
        ++tracker_->copy_assign;
        return *this;
    }
    TrackedType& operator=(TrackedType &&other) noexcept((std::is_same_v<NoexceptMoveAssignTag, Tags> || ...))
    {
        ++tracker_->move_assign;
        return *this;
    }
    // should be always noexcept, otherwise std::is_nothrow_move_constructible_v will return false,
    // whatever move ctor noexcept specifier
    ~TrackedType() noexcept
    {
        ++tracker_->dtor;
    }

private:
    const std::shared_ptr<TypeTracker> tracker_;
};
