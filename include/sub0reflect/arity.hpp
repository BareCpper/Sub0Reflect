#pragma once

#include <utility>
#include <tuple>
#include <array>

struct ubiq
{
    template <typename T>
    operator T();
};

template <std::size_t>
using ubiq_t = ubiq;

template <typename T, typename... Ubiqs>
constexpr auto arity_r(std::size_t& sz, int) -> std::void_t<decltype(T{Ubiqs{}...})>
{
    sz = sizeof...(Ubiqs); //< This arity is valid
}

template <typename T, typename, typename... Ubiqs>
constexpr auto arity_r(std::size_t& sz, float)
{
    arity_r<T, Ubiqs...>(sz, 0); //< Check for Arity-1
}

template<typename T, std::size_t... Is>
constexpr auto arity(std::index_sequence<Is...>)
{
    std::size_t sz = 0;
    arity_r<T, ubiq_t<Is>...>(sz, 0);
    return sz;
}

/**
@tparam MinmumMemberBits  Minimum number of bits for members - TDefualt:8, reduces compilation time
*/
template<typename T, std::size_t MinmumMemberBits = 8 >
constexpr auto arity()
{
    return arity<T>(std::make_index_sequence<(sizeof(T)*8) / MinmumMemberBits>{});
}

namespace test
{
    struct Zero {
    };

    struct One {
        int a = 1;
    };

    struct Two {
        int a;
        int b = 2;
    };

    struct Many {
        int a, b, c, d, e, f, g, h, i, j, k;
    };
    struct BitField {
        int a : 8;
        int b : 8;
        int b2 : 2;
        int c : 1;
        int d : 1;
        int e : 1;
        int f : 1;
    };

    struct Complex
    {
        std::array<char,32> a;
        char a2;
        std::array<char,32> b;
        float c;
        double d;
    };
}

static_assert(arity<test::Zero>() == 0);
static_assert(arity<test::One>() == 1);
static_assert(arity<test::Two>() == 2);
static_assert(arity<test::Many>() == 11);
static_assert(arity<test::BitField,1>() == 7);
static_assert(arity<test::Complex>() == 5);