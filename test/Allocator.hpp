#pragma once
#include <memory>
#include <type_traits>

namespace alloc
{
    template <typename T, typename Traits>
    struct Allocator
    {
        using value_type = T;

        // make sure that if is_always_equal=true, then operator== should returns true too
        using is_always_equal = typename Traits::is_always_equal;
        using propagate_on_container_move_assignment = typename Traits::pocma;
        using propagate_on_container_copy_assignment = typename Traits::pocca;

        template<typename U>
        struct rebind
        {
            using other = Allocator<U, Traits>;
        };

        T *allocate(std::size_t n) { return std::allocator<T>{}.allocate(n); }
        void deallocate(T *p, std::size_t s) { std::allocator<T>{}.deallocate(p, s); }

        friend bool operator==(const Allocator&, const Allocator&) noexcept { return Traits::equal; }
        friend bool operator!=(const Allocator&l, const Allocator&r) noexcept { return !(l == r); }
    };

    namespace traits_
    {
        struct StaticEq
        {
            constexpr static bool equal = true;
            using is_always_equal = std::true_type;
            using pocma = std::false_type;
            using pocca = std::false_type;
        };
        struct DynamicEq
        {
            constexpr static bool equal = true;
            using is_always_equal = std::false_type;
            using pocma = std::false_type;
            using pocca = std::false_type;
        };
        struct DynamicNotEq
        {
            constexpr static bool equal = false;
            using is_always_equal = std::false_type;
            using pocma = std::false_type;
            using pocca = std::false_type;
        };
        struct StaticEqPocma
        {
            constexpr static bool equal = true;
            using is_always_equal = std::true_type;
            using pocma = std::true_type;
            using pocca = std::false_type;
        };
        struct DynamicEqPocma
        {
            constexpr static bool equal = true;
            using is_always_equal = std::false_type;
            using pocma = std::true_type;
            using pocca = std::false_type;
        };
        struct DynamicNotEqPocma
        {
            constexpr static bool equal = false;
            using is_always_equal = std::false_type;
            using pocma = std::true_type;
            using pocca = std::false_type;
        };
        struct StaticEqPocca
        {
            constexpr static bool equal = true;
            using is_always_equal = std::true_type;
            using pocma = std::false_type;
            using pocca = std::true_type;
        };
        struct DynamicEqPocca
        {
            constexpr static bool equal = true;
            using is_always_equal = std::false_type;
            using pocma = std::false_type;
            using pocca = std::true_type;
        };
        struct DynamicNotEqPocca
        {
            constexpr static bool equal = false;
            using is_always_equal = std::false_type;
            using pocma = std::false_type;
            using pocca = std::true_type;
        };
    }

    template <typename T>
    using StaticEq = Allocator<T, traits_::StaticEq>;
    template <typename T>
    using DynamicEq = Allocator<T, traits_::DynamicEq>;
    template <typename T>
    using DynamicNotEq = Allocator<T, traits_::DynamicNotEq>;

    template <typename T>
    using StaticEqPocma = Allocator<T, traits_::StaticEqPocma>;
    template <typename T>
    using DynamicEqPocma = Allocator<T, traits_::DynamicEqPocma>;
    template <typename T>
    using DynamicNotEqPocma = Allocator<T, traits_::DynamicNotEqPocma>;

    template <typename T>
    using StaticEqPocca = Allocator<T, traits_::StaticEqPocca>;
    template <typename T>
    using DynamicEqPocca = Allocator<T, traits_::DynamicEqPocca>;
    template <typename T>
    using DynamicNotEqPocca = Allocator<T, traits_::DynamicNotEqPocca>;
}
