#include "sub0reflect/sub0reflect.hpp"

/// @note Test namespace
/// @test SUB0_REFLECT() can be used within a user-namespace i.e. No Global-namespace requirements
namespace test
{
    struct Point
    {
        float a;
        uint16_t b;
        int32_t c;
    };
    SUB0_REFLECT(Point, a, b, c);
    
    struct Triangle
    {
        Point a, b, c;
    };
    SUB0_REFLECT(Triangle, a, b, c); //< @note Expands to what is shown below

} //< END: test