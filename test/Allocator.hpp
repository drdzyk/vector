#pragma once
#include <memory>
#include <type_traits>

namespace alloc
{
    enum class Equal{Yes, No};
    enum class Pocma{Yes, No};
    enum class Pocca{Yes, No};

    template <typename T, Equal equal, Pocma pocma, Pocca pocca>
    struct Allocator
    {
        using value_type = T;

        using propagate_on_container_move_assignment = std::integral_constant<bool, pocma == Pocma::Yes>;
        using propagate_on_container_copy_assignment = std::integral_constant<bool, pocca == Pocca::Yes>;

        template<typename U>
        struct rebind
        {
            using other = Allocator<U, equal, pocma, pocca>;
        };

        T *allocate(std::size_t n) { return std::allocator<T>{}.allocate(n); }
        void deallocate(T *p, std::size_t s) { std::allocator<T>{}.deallocate(p, s); }

        friend bool operator==(const Allocator&, const Allocator&) noexcept { return equal == Equal::Yes; }
        friend bool operator!=(const Allocator&l, const Allocator&r) noexcept { return !(l == r); }
    };

    template <typename T>
    using Eq = Allocator<T, Equal::Yes, Pocma::No, Pocca::No>;
    template <typename T>
    using NotEq = Allocator<T, Equal::No, Pocma::No, Pocca::No>;

    template <typename T>
    using EqPocma = Allocator<T, Equal::Yes, Pocma::Yes, Pocca::No>;
    template <typename T>
    using NotEqPocma = Allocator<T, Equal::No, Pocma::Yes, Pocca::No>;

    template <typename T>
    using EqPocca = Allocator<T, Equal::Yes, Pocma::No, Pocca::Yes>;
    template <typename T>
    using NotEqPocca = Allocator<T, Equal::No, Pocma::No, Pocca::Yes>;
}
