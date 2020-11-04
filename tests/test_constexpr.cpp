#include "test_types.hpp"

#include "gtest/gtest.h"

constexpr const test::Point p = { 11, 22, 33 };



//static_assert(Point_Reflect::name() == "Point");

//static_assert(Point_Reflect::size() == sizeof(test::Point));

namespace Test
{
    TEST(constexpr, sub0_reflect_t_type)
    {
        using Reflect_t = sub0::reflect_t<test::Point>;

        static_assert(std::is_same_v<Reflect_t::type, test::Point>);

        EXPECT_TRUE(true); ///< @note static_assert pass
    }

    TEST(constexpr, sub0_reflect_t_name)
    {
        using Reflect_t = sub0::reflect_t<test::Point>;

        static_assert(Reflect_t::name() == "Point");

        EXPECT_TRUE(true); ///< @note static_assert pass
    }

    TEST(constexpr, sub0_reflect_t_size)
    {
        using Reflect_t = sub0::reflect_t<test::Point>;

        static_assert(Reflect_t::size() == sizeof(test::Point));

        EXPECT_TRUE(true); ///< @note static_assert pass
    }
}