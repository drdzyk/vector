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

template <typename T>
struct A {};

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

template <typename T> class X;
int main() {
//    using T = std::__replace_first_arg_t<A<int>, double>;
//    X<T> x;
//    using AllocDouble = Alloc<double>;
//    using Rebind = std::allocator_traits<AllocDouble>::rebind_alloc<int>;
//
//    X<Rebind> x;

    std::unordered_map<int, int> d;
    std::list<int, std::allocator<int>> l;

    low::vector<int, Alloc<int>> v{Alloc<int>{}};
    v.resize(12);

//    for (const auto &e : v)
//    {
//        std::cout << "\ne: " << e << std::endl;
//    }

    std::cout << "\nsizeof(v): " << sizeof(v) << std::endl;
    std::cout << "\ns: " << v.size() << "; c: " << v.capacity() << std::endl;
    v.resize(9);
    std::cout << "\ns: " << v.size() << "; c: " << v.capacity() << std::endl;
    v.resize(13 );
    std::cout << "\ns: " << v.size() << "; c: " << v.capacity() << std::endl;
//    v.resize(7);
//    std::cout << "\ns: " << v.size() << "; c: " << v.capacity() << std::endl;

    return 0;
}
