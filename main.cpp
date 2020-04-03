#include <cassert>
#include <iostream>
#include "vector.hpp"
#include <bits/ptr_traits.h>
#include <vector>
#include <list>
#include <unordered_map>


// TODO:
// 0) combine logic of reallocate storage
// 1) ctor - dtor sequence?
// 2) exception safety
// 3) call move ctor only when noexcept
// 4) test_exceptions.cpp: add std::vector
// 5) check AllocatorAwareContainer, Container, etc
// 6) check pocma, pocca
// 7) if value_type dtor throws -> terminate now(~vector() noexcept); same for clear(), shrink_to_fit()
// 8) emplace_back should return reference
// 9) add operator ==()
// 10) remove sanitizers from benchmark
// 11) use catch2?
// 12) somewhere std::distance, somewhere raw operations

template <typename T>
struct Alloc
{
    using value_type = T;

//    template <typename U>
//    struct rebind
//    {
//        using other = Alloc<U>;
////        using other = int;
//    };

    auto allocate(size_t __n, const void* = static_cast<const void*>(0))
    {
        return std::allocator<T>{}.allocate(__n);
    }
    void deallocate(value_type* __p, size_t)
    {
        std::allocator<T>{}.deallocate(__p, 0);
    }
};

struct A
{
    A() = default;
    A(int){}
//    bool operator ==(const A&) { return true; }
    friend bool operator ==(const A&, const A&) { return true; }

};

struct B : A
{

};
std::size_t global_tracker_alloc{0}, global_tracker_dealloc{0};
template <typename T>
struct NotEqualAllocator
{

    using value_type = T;

    T *allocate(std::size_t n)
    {
        ++global_tracker_alloc;
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T *p, std::size_t s)
    {
        ++global_tracker_dealloc;
        std::allocator<T>{}.deallocate(p, s);
    }
//    template<typename _Tp1>
//    struct rebind
//    { typedef NotEqualAllocator<_Tp1> other; };
//
//    NotEqualAllocator() = default;
//    template<typename _Tp1>
//    NotEqualAllocator(const NotEqualAllocator<_Tp1>&) noexcept { }

    friend bool operator==(const NotEqualAllocator&, const NotEqualAllocator&) noexcept { return false; }
    friend bool operator!=(const NotEqualAllocator&, const NotEqualAllocator&) noexcept { return true; }
};
template <typename T> class X;
int main() {

    low::vector<int, NotEqualAllocator<int>> src;
    std::cerr << "\nalloc  : " << global_tracker_alloc << std::endl;
    std::cerr << "\ndealloc: " << global_tracker_dealloc << std::endl;
    src.emplace_back(3);
    src.emplace_back(3);
    src.emplace_back(3);
    src.emplace_back(3);
    assert(src[0] == 3);
    assert(src[1] == 3);
    assert(src[2] == 3);
    assert(src[3] == 3);

    std::cerr << "\nalloc  : " << global_tracker_alloc << std::endl;
    std::cerr << "\ndealloc: " << global_tracker_dealloc << std::endl;

    low::vector<int, NotEqualAllocator<int>>::allocator_type other;
    low::vector<int, NotEqualAllocator<int>> copy{std::move(src), other};

    std::cerr << "\ncopy[0]: " << copy[0] << std::endl;
    std::cerr << "\ncopy[1]: " << copy[1] << std::endl;
    std::cerr << "\ncopy[2]: " << copy[2] << std::endl;
    std::cerr << "\ncopy[3]: " << copy[3] << std::endl;

    std::cerr << "\nalloc  : " << global_tracker_alloc << std::endl;
    std::cerr << "\ndealloc: " << global_tracker_dealloc << std::endl;
    assert(copy[0] == 3);
    assert(copy[1] == 3);
    assert(copy[2] == 3);
    assert(copy[3] == 3);


//
//    X<decltype(copy)::allocator_type> x;


    return 0;
}
