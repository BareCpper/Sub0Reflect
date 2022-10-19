#pragma once

#if 0
#define static_assert_equal(x, y) assert_equal( const_val<x>{}, const_val<y>{} );

template<auto& T>
struct const_val {
    constexpr const_val() = default;
};

template <int v>
struct display_non_zero_int_value;

template <>
struct display_non_zero_int_value<0> { static constexpr bool foo = true; };

static constexpr int v = 1;



template<auto& A, auto& B>
constexpr void assert_equal() {
    static_assert(A == B, "Values are not equal!");
}
#endif
