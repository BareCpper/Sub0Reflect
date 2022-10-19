#pragma once

#include "arity.hpp"

#define AS_TUPLE_IMPL( count, ... ) \
    template <typename T> \
    constexpr inline auto as_tuple_impl(T&& t, std::integral_constant<std::size_t, count>) \
    { \
        auto& [ __VA_ARGS__ ] = t; \
        return std::forward_as_tuple( __VA_ARGS__); \
    } 
    
AS_TUPLE_IMPL( 1, x0 );
AS_TUPLE_IMPL( 2, x0, x1 );
AS_TUPLE_IMPL( 3, x0, x1, x2 );
AS_TUPLE_IMPL( 4, x0, x1, x2, x3 );
AS_TUPLE_IMPL( 5, x0, x1, x2, x3, x4 );
AS_TUPLE_IMPL( 6, x0, x1, x2, x3, x4, x5 );
AS_TUPLE_IMPL( 7, x0, x1, x2, x3, x4, x5, x6 );
AS_TUPLE_IMPL( 8, x0, x1, x2, x3, x4, x5, x6, x7 );
AS_TUPLE_IMPL( 9, x0, x1, x2, x3, x4, x5, x6, x7, x8 );
AS_TUPLE_IMPL( 10, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9 );

//Empty specialisation
template <typename T>
constexpr inline auto as_tuple_impl(T&& t, std::integral_constant<std::size_t, 0>) \
{ 
    return std::make_tuple(); 
} 

template <typename T>
constexpr inline auto as_tuple(T&& var )
{
    using rT = std::remove_reference_t<T>;
    return as_tuple_impl( var, std::integral_constant<std::size_t, arity<rT>()>{});
}
