#include "Allocator.hpp"

namespace alloc
{
    using A_ = StaticEq<int>;
    static_assert(A_::is_always_equal::value);
    static_assert(!A_::propagate_on_container_copy_assignment::value);
    static_assert(!A_::propagate_on_container_move_assignment::value);
    static_assert(A_{} == A_{});

    using B_ = DynamicEq<int>;
    static_assert(!B_::is_always_equal::value);
    static_assert(!B_::propagate_on_container_copy_assignment::value);
    static_assert(!B_::propagate_on_container_move_assignment::value);
    static_assert(B_{} == B_{});

    using C_ = DynamicNotEq<int>;
    static_assert(!C_::is_always_equal::value);
    static_assert(!C_::propagate_on_container_copy_assignment::value);
    static_assert(!C_::propagate_on_container_move_assignment::value);
    static_assert(C_{} != C_{});

    using D_ = StaticEqPocma<int>;
    static_assert(D_::is_always_equal::value);
    static_assert(!D_::propagate_on_container_copy_assignment::value);
    static_assert(D_::propagate_on_container_move_assignment::value);
    static_assert(D_{} == D_{});

    using E_ = DynamicEqPocma<int>;
    static_assert(!E_::is_always_equal::value);
    static_assert(!E_::propagate_on_container_copy_assignment::value);
    static_assert(E_::propagate_on_container_move_assignment::value);
    static_assert(E_{} == E_{});

    using F_ = DynamicNotEqPocma<int>;
    static_assert(!F_::is_always_equal::value);
    static_assert(!F_::propagate_on_container_copy_assignment::value);
    static_assert(F_::propagate_on_container_move_assignment::value);
    static_assert(F_{} != F_{});

    using G_ = StaticEqPocca<int>;
    static_assert(G_::is_always_equal::value);
    static_assert(G_::propagate_on_container_copy_assignment::value);
    static_assert(!G_::propagate_on_container_move_assignment::value);
    static_assert(G_{} == G_{});

    using H_ = DynamicEqPocca<int>;
    static_assert(!H_::is_always_equal::value);
    static_assert(H_::propagate_on_container_copy_assignment::value);
    static_assert(!H_::propagate_on_container_move_assignment::value);
    static_assert(H_{} == H_{});

    using I_ = DynamicNotEqPocca<int>;
    static_assert(!I_::is_always_equal::value);
    static_assert(I_::propagate_on_container_copy_assignment::value);
    static_assert(!I_::propagate_on_container_move_assignment::value);
    static_assert(I_{} != I_{});

    using J_ = WrongEq<int>;
    static_assert(J_::is_always_equal::value);
    static_assert(!J_::propagate_on_container_copy_assignment::value);
    static_assert(!J_::propagate_on_container_move_assignment::value);
    static_assert(J_{} != J_{});
}
