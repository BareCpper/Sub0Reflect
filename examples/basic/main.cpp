#include <iostream>

#include "sub0reflect/sub0reflect.hpp"

#if 0


namespace test
{

    struct Point
    {
        float a;
        uint16_t b;
        int32_t c;
    };

#if 1
    SUB0_REFLECT(Point, a, b, c); //< @note Expands to what is shown below
#else
    struct sub0_Reflect_Point
    {
        using a = sub0::MemberTie<decltype(Point::a) Point::*, &Point::a>;
        [[nodiscard]] static constexpr std::string_view name(a) noexcept { return "a"; }

        using b = sub0::MemberTie<decltype(Point::b) Point::*, &Point::b>;
        [[nodiscard]] static constexpr std::string_view name(b) noexcept { return "b"; }

        using c = sub0::MemberTie<decltype(Point::c) Point::*, &Point::c>;
        [[nodiscard]] static constexpr std::string_view name(c) noexcept { return "c"; }

        [[nodiscard]] static constexpr auto members() { return std::tuple<a, b, c>(); }
        [[nodiscard]] static constexpr std::string_view name() { return "Point"; };
        [[nodiscard]] static constexpr std::size_t size() { return sizeof(Point); };
    };
    constexpr sub0_Reflect_Point sub0_reflect(Point) { return {}; };
#endif

    struct Triangle
    {
        Point a, b, c;
    };
    
#if 1
    SUB0_REFLECT(Triangle, a, b, c); //< @note Expands to what is shown below
#else
    struct sub0_Reflect_Triangle
    {
        using type = sub0_Reflect_Triangle;

        using a = sub0::MemberTie<decltype(Triangle::a) Triangle::*, &Triangle::a>;
        [[nodiscard]] static constexpr std::string_view name(a) noexcept { return "a"; }

        using b = sub0::MemberTie<decltype(Triangle::b) Triangle::*, &Triangle::b>;
        [[nodiscard]] static constexpr std::string_view name(b) noexcept { return "b"; }

        using c = sub0::MemberTie<decltype(Triangle::c) Triangle::*, &Triangle::c>;
        [[nodiscard]] static constexpr std::string_view name(c) noexcept { return "c"; }

        [[nodiscard]] static constexpr auto members() { return std::tuple<a, b, c>(); }
        [[nodiscard]] static constexpr std::string_view name() { return "Triangle"; };
        [[nodiscard]] static constexpr std::size_t size() { return sizeof(Triangle); };
    };
    constexpr sub0_Reflect_Triangle sub0_reflect(Triangle) { return {}; };
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


#endif
int main ()
{

#if 0
    using namespace test;
    const Point p = { 11, 22, 33 };

    auto p_Reflect = sub0::reflect(p);

   // auto p_tied = sub0::tie(cP, pointReflection.members())

    static_assert(p.a == 11);
    static_assert(sub0::tie<decltype(p_Reflect)::a>(p) == 11);
    static_assert(decltype(p_Reflect)::a().name() == std::string_view("a"));



  /*  std::apply([](auto&... member) {
        ...
        }, p_tied);*/

    Point_Reflect::a a;
    std::cout << a.name();
    //static_assert(p_tied)
#endif

#if 0
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

#endif

    return 0;// a(p) + b(p);
}
