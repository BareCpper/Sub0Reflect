#include "cppReflect17/cppReflect17.hpp"
#include "gtest/gtest.h"

namespace NS1 {
    namespace NS2 {
        namespace NS3 { 
            struct Foo
            {
                int bar;
                float baz;
            };

            struct NotReflection
            {
                int a, b, c;
            };
        } 
    } 
}

CPPREFLECT17_REFLECT(NS1::NS2::NS3::Foo, bar, baz);

#if 0 //, @todo free functions short interface...?
constexpr auto ta = cppReflect17::name<NS1::NS2::NS3::Foo>();
static_assert(ta == "Foo");
constexpr auto ta_mem = cppReflect17::members<NS1::NS2::NS3::Foo>();
#endif

static_assert(true == cppReflect17::hasExplicitReflection<NS1::NS2::NS3::Foo>());
static_assert(false == cppReflect17::hasExplicitReflection<NS1::NS2::NS3::NotReflection>());

#if 0 // @todo We should also support automatic reflection (i.e. Structured binding approach @see `as_tuple`)
static_assert(true == cppReflect17::hasImplicitReflection<NS1::NS2::NS3::NotReflection>());
#endif

#if 0 // @todo Check for undocumented explicit members i.e. Some members are only 'implicit'.. requires support automatic reflection
static_assert(true == cppReflect17::isPartial<NS1::NS2::NS3::Foo>());
static_assert(false == cppReflect17::isPartial<NS1::NS2::NS3::NotReflection>());
#endif

constexpr auto reflection = cppReflect17::reflect<NS1::NS2::NS3::Foo>{};
static_assert(reflection.name() == "NS1::NS2::NS3::Foo");
constexpr auto mems = reflection.members();
static_assert(std::tuple_size<decltype(mems)>() == 2);

/// Mirror - From Reflection
constexpr auto mirror = reflection.mirror();
static_assert(mirror.bar.name() == "bar");
static_assert(mirror.baz.name() == "baz");

/// Index of Member - By Name
constexpr auto iPpgMem = reflection.index("bar");
constexpr auto iProximityMem = reflection.index("baz");
static_assert(iPpgMem == 0);
static_assert(iProximityMem == 1);

/// Member-Names - From Reflection
constexpr auto memNames = reflection.memberNames();
static_assert(std::get<0>(mems).name() == std::get<0>(memNames));
static_assert(std::get<1>(mems).name() == std::get<1>(memNames));
static_assert(memNames[iPpgMem] == "bar" && memNames[iProximityMem] == "baz");

constexpr NS1::NS2::NS3::Foo testData = { 123,456 };

/// Tie Member - From Members
static_assert(std::get<iPpgMem>(mems)(testData) == testData.bar, "Tied value == input instance");
static_assert(std::get<iProximityMem>(mems)(testData) == testData.baz, "Tied value== input instance");
static_assert(&std::get<iPpgMem>(mems)(testData) == &testData.bar, "Tied value is reference to input instance");
static_assert(&std::get<iProximityMem>(mems)(testData) == &testData.baz, "Tied value is reference to input instance");

/// Bind - From Reflection
constexpr auto bound = reflection(testData);
static_assert(&bound.instance == &testData);

/// Tie Member - From Bound
constexpr auto boundMems = bound.members();
static_assert(*std::get<iPpgMem>(boundMems) == testData.bar, "Tied value == input instance");
static_assert(*std::get<iProximityMem>(boundMems) == testData.baz, "Tied value == input instance");
static_assert(&*std::get<iPpgMem>(boundMems) == &testData.bar, "Tied value is reference to input instance");
static_assert(&*std::get<iProximityMem>(boundMems) == &testData.baz, "Tied value is reference to input instance");

/// Tie Member - From Mirror
static_assert(mirror.bar(testData) == testData.bar, "Tied value ==input instance");
static_assert(mirror.baz(testData) == testData.baz, "Tied value ==input instance");
static_assert(&mirror.bar(testData) == &testData.bar, "Tied value is reference to input instance");
static_assert(&mirror.baz(testData) == &testData.baz, "Tied value is reference to input instance");

/// Tie as Mirror - From Reflection
constexpr auto  reflectionMirror = reflection.mirror(testData);
static_assert(*reflectionMirror.bar == testData.bar, "Tied value == input instance");
static_assert(*reflectionMirror.baz == testData.baz, "Tied value == input instance");
static_assert(&*reflectionMirror.bar == &testData.bar, "Tied value is reference to input instance");
static_assert(&*reflectionMirror.baz == &testData.baz, "Tied value  is reference to input instance");

/// Tie as Mirror - From Bound
constexpr auto  boundMirror = bound.mirror();
static_assert(*boundMirror.bar == testData.bar, "Tied value == input instance");
static_assert(*boundMirror.baz == testData.baz, "Tied value == input instance");
static_assert(&*boundMirror.bar == &testData.bar, "Tied value is reference to input instance");
static_assert(&*boundMirror.baz == &testData.baz, "Tied value  is reference to input instance");

/// Ref as Mirror - From Reflection
constexpr auto reflectionRef = reflection.ref(testData);
static_assert(reflectionRef.bar == testData.bar, "Tied value shall match input instance");
static_assert(reflectionRef.baz == testData.baz, "Tied value shall match input instance");
static_assert(&reflectionRef.bar == &testData.bar, "Tied value is reference to input instance");
static_assert(&reflectionRef.baz == &testData.baz, "Tied value  is reference to input instance");

/// Ref as Mirror - From Bound
constexpr auto boundRef = bound.ref();
static_assert(boundRef.bar == testData.bar, "Tied value shall match input instance");
static_assert(boundRef.baz == testData.baz, "Tied value shall match input instance");
static_assert(&boundRef.bar == &testData.bar, "Tied value is reference to input instance");
static_assert(&boundRef.baz == &testData.baz, "Tied value  is reference to input instance");

#if 0 ///< @TODO Compile time visit()?
static_assert(bound.visit("bar", [=](auto& val) { return &val == &testData.bar; }), "Visited value is reference to input instance");
#endif

TEST(cppReflect17, constexpr)
{
    EXPECT_TRUE(true); ///< @note constexpr passes by fact of compilation success
}