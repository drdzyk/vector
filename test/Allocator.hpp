#pragma once
#include <memory>

template <typename T>
struct ComparableAllocator
{
    using value_type = T;

    T *allocate(std::size_t n) { return std::allocator<T>{}.allocate(n); }
    void deallocate(T *p, std::size_t s) { std::allocator<T>{}.deallocate(p, s); }
};

template <typename T>
struct EqualAllocator : ComparableAllocator<T>
{
    friend bool operator==(const EqualAllocator&, const EqualAllocator&) noexcept { return true; }
    friend bool operator!=(const EqualAllocator&l, const EqualAllocator&r) noexcept { return !(l == r); }
};

template <typename T>
struct NotEqualAllocator : ComparableAllocator<T>
{
    friend bool operator==(const NotEqualAllocator&, const NotEqualAllocator&) noexcept { return false; }
    friend bool operator!=(const NotEqualAllocator&l, const NotEqualAllocator&r) noexcept { return !(l == r); }
};
