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
// 13) move assign op: use move iterator only when move ctor/assign noexcept
// 14) clang-9 + c++2a + -fmodules
// 15) emplace_back tracker test

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

struct A
{
    A(int i) : i_(i)
    {
        std::cerr << "ctor: " << i_ << std::endl;
    }
    A(const A &r) : i_(r.i_)
    {
        std::cerr << "copy_ctor: " << i_ << std::endl;
//                throw std::runtime_error{"hello"};

    }
    A(A &&r): i_(r.i_)
    {
        r.i_ = 0;
        std::cerr << "move_ctor: " << i_ << std::endl;
//        throw std::runtime_error{"hello"};
    }
    A& operator=(const A &r)
    {
        i_ = r.i_;
        std::cerr << "copy_assign: " << i_ << std::endl;
//        throw std::runtime_error{"hello"};

        return *this;
    }
    A& operator=(A &&r)
    {
        i_ = r.i_;
        std::cerr << "move_assign: " << i_ << std::endl;
//        throw std::runtime_error{"hello"};

        return *this;
    }
    ~A()
    {
        std::cerr << "dtor: " << i_ << std::endl;
    }

    int i_{0};
};

template <typename T> class X;
int main() {
    std::vector<A> s;
    s.emplace_back(11);

    std::vector<A> v;
//    v.reserve(3);
    v.emplace_back(7);
    v.emplace_back(8);

    try
    {
        v.insert(std::next(v.begin()), s.begin(), s.end());
//        v.insert(std::next(v.begin()), std::move_iterator{s.begin()}, std::move_iterator{s.end()});
//        v.insert(v.end(), s.begin(), s.end());
    }
    catch (...)
    {

    }

    for (const auto &n : v)
    {
        std::cerr << n.i_ << " ";
    }
    std::cerr << std::endl;

    return 0;
}
