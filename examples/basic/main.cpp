#include <iostream>

#include "sub0bus/sub0bus.hpp"



namespace test
{

    struct Point
    {
        float a;
        uint16_t b;
        int32_t c;
    };


    struct sub0_Reflect_Point
    {
        using a = sub0::MemberTie<decltype(Point::a) Point::*, &Point::a>;
        [[nodiscard]] constexpr std::string_view name(a) noexcept { return "a"; }

        using b = sub0::MemberTie<decltype(Point::b) Point::*, &Point::b>;
        [[nodiscard]] constexpr std::string_view name(b) noexcept { return "b"; }

        using c = sub0::MemberTie<decltype(Point::c) Point::*, &Point::c>;
        [[nodiscard]] constexpr std::string_view name(c) noexcept { return "c"; }

        [[nodiscard]] static constexpr auto members() { return std::tuple<a, b, c>(); }
    };
    constexpr sub0_Reflect_Point reflect(Point) { return {}; };
    constexpr std::string_view name(sub0_Reflect_Point) { return "Point"; };


    struct Triangle
    {
        Point a, b, c;
    };


    #define SUB0_FIRST_ARG(arg,...) arg
#define SUB0_STRINGIFY( a ) #a 

    #define SUB0_REFLECT_FIELD( member ) \
        using member = sub0::MemberTie<decltype(type_t::member) type_t::*, &type_t::member>; \
        [[nodiscard]] constexpr std::string_view name(member) noexcept { return SUB0_STRINGIFY(member); }

    #define sub0_reflect( T, ... ) \
        struct sub0_Reflect_##T { \
            using type_t = T; \
            [[nodiscard]] static constexpr auto members() { return std::tuple<__VA_ARGS__>(); } \
            SUB0_REFLECT_FIELD( SUB0_FIRST_ARG(__VA_ARGS__) )


    #define SUB0_REFLECT_END( T ) \
    }; \
    constexpr sub0_Reflect_##T reflect(T) { return {}; }; \
    constexpr std::string_view name(sub0_Reflect_##T) { return #T; };

#if 1
    sub0_reflect(Triangle, a, b, c); //< @todo Expnd to to that shown below!
  
    SUB0_REFLECT_FIELD(b);
    SUB0_REFLECT_FIELD(c);
    SUB0_REFLECT_END(Triangle);
#else
    struct sub0_Reflect_Triangle
    {
        using a = sub0::MemberTie<decltype(Triangle::a) Triangle::*, &Triangle::a>;
        [[nodiscard]] constexpr std::string_view name(a) noexcept { return "a"; }

        using b = sub0::MemberTie<decltype(Triangle::b) Triangle::*, &Triangle::b>;
        [[nodiscard]] constexpr std::string_view name(b) noexcept { return "b"; }

        using c = sub0::MemberTie<decltype(Triangle::c) Triangle::*, &Triangle::c>;
        [[nodiscard]] constexpr std::string_view name(c) noexcept { return "c"; }

        [[nodiscard]] static constexpr auto members() { return std::tuple<a, b, c>(); }
    };
    constexpr sub0_Reflect_Triangle reflect(Triangle) { return {}; };
    constexpr std::string_view name(sub0_Reflect_Triangle) { return "Triangle"; };
#endif
}

template< typename T >
void testReflectPrint( T& p )
{
    constexpr auto reflection = sub0::reflect<T>();
    auto p_tied = sub0::tie(p, reflection.members());
    std::cout << name(reflection) << std::endl;
    std::apply([&p](auto&... members) {
        char* delim = "";
        ((std::cout << delim
            << members.name() << '|'
            << name(members) << '|'
            << sub0::name(members)
            << '='
            << members << '|'
            << members(p) << '|'
            << tie(p, members) << '|'
            << sub0::tie(p, members), delim = ", "), ...);
        std::cout << std::endl;
        }, p_tied);
}


template< typename T >
void testReflectPrintEx(T& p)
{
    constexpr auto reflection = sub0::reflect<T>();
    auto p_tied = sub0::tie(p, reflection.members());
    std::cout << name(reflection) << std::endl;
    std::apply([&p](auto&... members) {
        char* delim = "";
        ((std::cout << delim
            << name(members) 
            << '=', 
            testReflectPrint(*members), delim = ", "), ...);
        std::cout << std::endl;
        }, p_tied);
}


int main ()
{
    using namespace test;
       
    const Point p = { 11, 22, 33 };
    std::tuple<Point, const Point, Point> points =
    {
        { 11, 22, 33 }
        , { 44, 55, 66 }
        , { 77, 88, 99 }
    };

    std::apply([](auto&... p) {
        (testReflectPrint(p),...);
        }, points);


    std::tuple<Triangle, const Triangle> tris =
    {
         { { 11, 22, 33 }
         , { 44, 55, 66 }
         , { 77, 88, 99 } }

       ,  { { -11, 22, -33 }
         , { -44, 55, -66 }
         , { -77, 88, -99 } }
    };

    std::apply([](auto&... p) {
        (testReflectPrintEx(p), ...);
        }, tris);

    return 0;// a(p) + b(p);
}

