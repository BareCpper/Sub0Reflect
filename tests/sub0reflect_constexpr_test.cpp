#include "sub0reflect/sub0reflect.hpp"
#include "gtest/gtest.h"

namespace NS1 {
    namespace NS2 {
        namespace NS3 { 
            struct FullReflected
            {
                int foo;
                unsigned int bar;
                float baz;
            };

            struct PartialReflected
            {
                int foo;
                unsigned int bar; //< NOT Reflected
                float baz;
            };

            struct NotReflection
            {
                int a, b, c;
            };

            template<bool option>
            struct TemplateReflection
            {
                int foo, bar, baz;
            };
        } 
    } 
}

SUB0REFLECT_REFLECT(NS1::NS2::NS3::FullReflected, foo, bar, baz);
SUB0REFLECT_REFLECT(NS1::NS2::NS3::PartialReflected, foo, /* @note `bar` is NOT reflected*/ baz); 
SUB0REFLECT_REFLECT(NS1::NS2::NS3::TemplateReflection<true>, foo, bar, baz);
SUB0REFLECT_REFLECT(NS1::NS2::NS3::TemplateReflection<false>, foo, bar, baz);

#if 0 //, @todo free functions short interface...?
constexpr auto ta = sub0reflect::name<NS1::NS2::NS3::FullReflected>();
static_assert(ta == "FullReflected");
constexpr auto ta_mem = sub0reflect::members<NS1::NS2::NS3::FullReflected>();
#endif

static_assert(true == sub0reflect::hasExplicitReflection<NS1::NS2::NS3::FullReflected>());
static_assert(false == sub0reflect::hasExplicitReflection<NS1::NS2::NS3::NotReflection>());

#if 0 // @todo We should also support automatic reflection (i.e. Structured binding approach @see `as_tuple`)
static_assert(true == sub0reflect::hasImplicitReflection<NS1::NS2::NS3::NotReflection>());
#endif

#if 0 // @todo Check for undocumented explicit members i.e. Some members are only 'implicit'.. requires support automatic reflection
static_assert(true == sub0reflect::isPartial<NS1::NS2::NS3::FullReflected>());
static_assert(false == sub0reflect::isPartial<NS1::NS2::NS3::NotReflection>());
#endif

constexpr auto reflection = sub0reflect::reflect<NS1::NS2::NS3::FullReflected>{};
static_assert(reflection.name() == "NS1::NS2::NS3::FullReflected");

constexpr auto partialReflection = sub0reflect::reflect<NS1::NS2::NS3::PartialReflected>{};
static_assert(partialReflection.name() == "NS1::NS2::NS3::PartialReflected");

/// Member-Names - From Reflection
constexpr auto memNames = reflection.memberNames();
static_assert(std::get<0>(memNames) == "foo");
static_assert(std::get<1>(memNames) == "bar");
static_assert(std::get<2>(memNames) == "baz");

#if 0 // @todo We should also support automatic reflection (i.e. Structured binding approach @see `as_tuple`)
constexpr auto partialMemNames = partialReflection.memberNames();
#endif

constexpr auto mems = reflection.members();
static_assert(std::tuple_size<decltype(mems)>() == 3);
static_assert(std::get<0>(mems).name() == std::get<0>(memNames));
static_assert(std::get<1>(mems).name() == std::get<1>(memNames));
static_assert(std::get<2>(mems).name() == std::get<2>(memNames));

/// Mirror - From Reflection
constexpr auto mirror = reflection.mirror();
static_assert(mirror.foo.name() == std::get<0>(memNames));
static_assert(mirror.bar.name() == std::get<1>(memNames));
static_assert(mirror.baz.name() == std::get<2>(memNames));

/// Index of Member - By Name
static_assert(reflection.index(std::get<0>(memNames)) == 0);
static_assert(reflection.index(std::get<1>(memNames)) == 1);
static_assert(reflection.index(std::get<2>(memNames)) == 2);

constexpr NS1::NS2::NS3::FullReflected testData = { 123,456 };

/// Tie Member - From Members
static_assert(std::get<0>(mems)(testData) == testData.foo, "Tied value == input instance");
static_assert(std::get<1>(mems)(testData) == testData.bar, "Tied value == input instance");
static_assert(&std::get<0>(mems)(testData) == &testData.foo, "Tied value is reference to input instance");
static_assert(&std::get<1>(mems)(testData) == &testData.bar, "Tied value is reference to input instance");

/// Bind - From Reflection
constexpr auto bound = reflection(testData);
static_assert(&bound.instance == &testData);

/// Tie Member - From Bound
constexpr auto boundMems = bound.members();
static_assert(*std::get<1>(boundMems) == testData.bar, "Tied value == input instance");
static_assert(*std::get<2>(boundMems) == testData.baz, "Tied value == input instance");
static_assert(&*std::get<1>(boundMems) == &testData.bar, "Tied value is reference to input instance");
static_assert(&*std::get<2>(boundMems) == &testData.baz, "Tied value is reference to input instance");

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

TEST(sub0reflect, constexpr)
{
    EXPECT_TRUE(true); ///< @note constexpr passes by fact of compilation success
}