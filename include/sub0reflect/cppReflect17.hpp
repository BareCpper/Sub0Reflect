#pragma once

#include "arity.hpp"
#include "as_tuple.hpp"
#include "preprocessor.hpp"
#include "type_name.hpp"
#include "static_assert_equal.hpp"

#include <cstdlib>
#include <memory> //< std::unique_ptr
#include <stdexcept> //< std::invalid_argument

#ifdef __GNUG__
    #include <cxxabi.h> //< __cxa_demangle
#endif


namespace cppReflect17 {
namespace internal {
    /** Type 'mirrors' the reflected struct with named members `struct membertie_type_mirror<T>{ using mem1 = MemberTie<T...>, ...; }`
    * @code
    *   membertie_type_mirror<T> foo; 
    *   static_assert( foo.member1.name() == "member1" )
    * @endcode
    */
    template< typename T>
    struct membertie_type_mirror;

    /** Meta data storage type
    * @remark Stores all meta-data that cannot be determined at compile time and is provided via user Macros `CPPREFLECT17_REFLECT_IMPL`
    */
    template< typename T>
    struct member_meta { 
        static constexpr bool hasExplicit = false;
    };

    template <typename T, T> struct MemberTie;
    template <typename T, T> struct InstanceMemberTie;
    template <typename T, T> struct ConstInstanceMemberTie;

    template <typename Member_t, typename Owner_t, Member_t(Owner_t::* MemPtr)>
    struct MemberTie< Member_t(Owner_t::*), MemPtr>
    {
        using this_t = MemberTie;
        using type_t = Member_t;
        using owner_t = Owner_t;
        using tie_t = InstanceMemberTie<Member_t(Owner_t::*), MemPtr>;
        using const_tie_t = ConstInstanceMemberTie<Member_t(Owner_t::*), MemPtr>;
        /** Get members name
        */
        [[nodiscard]] static constexpr std::string_view name() noexcept {
            return member_meta<owner_t>::name(this_t{});
        }

        [[nodiscard]] static constexpr const_tie_t tie(const owner_t& instance ) noexcept
        {
            return { instance };
        }

        [[nodiscard]] static constexpr tie_t tie( owner_t& instance) noexcept
        {
            return { instance };
        }

        [[nodiscard]] static constexpr const type_t& ref(const owner_t& instance) noexcept
        {
            return *tie(instance);
        }

        [[nodiscard]] static constexpr type_t& ref(owner_t& instance) noexcept
        {
            return *tie(instance);
        }

    public: //< Instance functions

        /** Reference members value on instance
        */
        [[nodiscard]] constexpr type_t& operator()(owner_t& instance) noexcept
        {
            return instance.*MemPtr;
        }

        /** Reference members value on instance
        */
        [[nodiscard]] constexpr const type_t& operator()(const owner_t& instance) const noexcept
        {
            return instance.*MemPtr;
        }
    };

    template <typename Member_t, typename Owner_t, Member_t(Owner_t::* MemPtr)>
    struct InstanceMemberTie< Member_t(Owner_t::*), MemPtr> : MemberTie< Member_t(Owner_t::*), MemPtr>
    {
        using this_t = InstanceMemberTie;
        using type_t = Member_t;
        using owner_t = Owner_t;

        owner_t& instance;

        constexpr InstanceMemberTie(owner_t& instance)
            : instance(instance)
        {}

        /** Reference members value on instance
        */
        [[nodiscard]] constexpr type_t& operator()() noexcept
        {
            return instance.*MemPtr;
        }

        [[nodiscard]] constexpr type_t& operator*() noexcept
        {
            return instance.*MemPtr;
        }

        /** Reference members value on instance
        */
        [[nodiscard]] constexpr const type_t& operator()() const noexcept
        {
            return instance.*MemPtr;
        }

        [[nodiscard]] constexpr const type_t& operator*() const noexcept
        {
            return instance.*MemPtr;
        }
    };
    template <typename Member_t, typename Owner_t, Member_t(Owner_t::* MemPtr)>
    struct ConstInstanceMemberTie< Member_t(Owner_t::*), MemPtr> : MemberTie< Member_t(Owner_t::*), MemPtr>
    {
        using this_t = ConstInstanceMemberTie;
        using type_t = Member_t;
        using owner_t = Owner_t;

        const owner_t& instance;

        constexpr ConstInstanceMemberTie(const owner_t& instance)
            : instance(instance)
        {}

        /** Reference members value on instance
        */
        [[nodiscard]] constexpr const type_t& operator()() const noexcept
        {
            return instance.*MemPtr;
        }

        [[nodiscard]] constexpr const type_t& operator*() const noexcept
        {
            return instance.*MemPtr;
        }
    };

    /** GCC uses mangled type names for `typeid(foo).name()` e.g. `foo::bar` is reported as `N3foo3barE`
    * ... No-op demangle when not GCC
    */
    std::string demangle(std::string_view name)
    {
#ifdef __GNUG__
        int status = -1; // @note Not '0' as this indicates success
        std::unique_ptr<char, void(*)(void*)> res{
            abi::__cxa_demangle(name.data(), NULL, NULL, &status),
            std::free
        };
        return (status == 0) ? res.get() : name.data();
#else
        return name.data(); //< no-op
#endif
    }

} //<END: internal
} //<END: cppReflect17

/**
 * @code
 * CPPREFLECT17_REFLECT( Type, {members...} )
 * @endcode 
 */
#ifndef CPPREFLECT17_REFLECT
#if 0
template <template <typename> typename Transformer, typename... Ts>
auto transform_types(std::tuple<Ts...>)->std::tuple<typename Transformer<Ts>::type...>;

template <template <typename> typename Transformer, typename Tuple>
using transform_types_t = decltype(transform_types<Transformer>(std::declval<Tuple>()));
#endif

#define CPPREFLECT17_MEMBER( MEMBER ) using MEMBER = cppReflect17::internal::MemberTie<decltype(type_t::MEMBER) type_t::*, &type_t::MEMBER>;

#define CPPREFLECT17_MEMBERMIRROR( MEMBER ) members_t::MEMBER MEMBER = {};
#define CPPREFLECT17_MEMBERMIRROR_TIE( MEMBER ) members_t::MEMBER::tie_t MEMBER;
#define CPPREFLECT17_MEMBERMIRROR_REF( MEMBER ) members_t::MEMBER::type_t& MEMBER;
#define CPPREFLECT17_MEMBERMIRROR_CONSTTIE( MEMBER ) members_t::MEMBER::const_tie_t MEMBER;
#define CPPREFLECT17_MEMBERMIRROR_CONSTREF( MEMBER ) const members_t::MEMBER::type_t& MEMBER;

#define CPPREFLECT17_MEMBER_NAME( MEMBER ) \
    [[nodiscard]] static constexpr std::string_view name(members_t::MEMBER) noexcept  { return CPPREFLECT17_STRINGIFY(MEMBER); }

#define CPPREFLECT17_REFLECT_IMPL( T, Tname, Tuuid, ... )                                                   \
    namespace cppReflect17 { namespace internal {                                                           \
        template <> struct membertie_type_mirror<T> {                                                       \
            using type_t = T;                                                                               \
            struct members_t{ CPPREFLECT17_MAP(CPPREFLECT17_MEMBER, __VA_ARGS__); };                        \
        };                                                                                                  \
        template <> struct member_meta<T> {                                                                 \
            using type_t = T;                                                                               \
            static constexpr bool hasExplicit = true;                                                       \
            using members_t = membertie_type_mirror<type_t>::members_t;                                     \
            struct mirror{ CPPREFLECT17_MAP(CPPREFLECT17_MEMBERMIRROR, __VA_ARGS__); };                     \
            struct mirror_tie{ CPPREFLECT17_MAP(CPPREFLECT17_MEMBERMIRROR_TIE, __VA_ARGS__); };             \
            struct mirror_ref{ CPPREFLECT17_MAP(CPPREFLECT17_MEMBERMIRROR_REF, __VA_ARGS__); };             \
            struct mirror_const_tie{ CPPREFLECT17_MAP(CPPREFLECT17_MEMBERMIRROR_CONSTTIE, __VA_ARGS__); };  \
            struct mirror_const_ref{ CPPREFLECT17_MAP(CPPREFLECT17_MEMBERMIRROR_CONSTREF, __VA_ARGS__); };  \
            [[nodiscard]] static constexpr std::string_view name() { return #Tname; }                       \
            CPPREFLECT17_MAP(CPPREFLECT17_MEMBER_NAME, __VA_ARGS__);                                        \
            struct tuples : private members_t {  using all = std::tuple<__VA_ARGS__>;  };                   \
        };                                                                                                  \
    } /* END: internal*/ } /* END: cppReflect17*/


#define CPPREFLECT17_REFLECT( T, ... ) \
    CPPREFLECT17_REFLECT_IMPL( T, T, T, __VA_ARGS__ )

/** Provide Tname when T is template e.g. `Foo<1>` is not a valid NS name so `Tname` can be set to `Foo_1`
*/
#define CPPREFLECT17_REFLECT_T( T, Tname, ... ) \
    CPPREFLECT17_REFLECT_IMPL( T, Tname, Tname, __VA_ARGS__ )

#endif

/** cppReflect17 top-level namespace
*/
namespace cppReflect17
{

    template< typename T>
    struct reflect;

    template< typename T>
    struct tie : reflect< std::remove_const_t<T> >
    {
        using type_t = T;
        using reflect_t = reflect< std::remove_const_t<T> >;

        type_t& instance;

        constexpr tie(type_t& instance)
            : instance(instance) 
        {}

        [[nodiscard]] constexpr auto members() const noexcept
        {
            return std::apply([this](auto... member) { 
                return std::tuple{ member.tie(instance)... }; 
            }, reflect_t::members());
        }

        [[nodiscard]] constexpr auto mirror() const noexcept
        {
            return reflect_t::mirror(instance);
        }

        [[nodiscard]] constexpr auto ref() const noexcept
        {
            return reflect_t::ref(instance);
        }

#if 0 ///< @TODO Compile time visit()?
        template< typename Function >
        [[nodiscard]] constexpr auto visit(const std::string_view& memberName, const Function& visitor) const noexcept
        {
            constexpr std::string_view t = "ppg";
            auto found = std::apply( [t](auto... member) {
                return  std::tuple_cat(std::conditional_t<decltype(member)::name() == t,
                          std::tuple<decltype(member)>,
                          std::tuple<>>{}...);
            }, reflect_t::members());

            return visitor(*std::get<0>(found).tie(instance));
        }
#endif
    };

    template< typename T>
    struct reflect 
    {
        using type_t = T;
        using members_t = typename internal::member_meta<T>::tuples::all;

        [[nodiscard]] static constexpr auto name() noexcept
        {
            return internal::member_meta<T>::name();
        }

        [[nodiscard]] static constexpr auto members() noexcept
        { 
            return members_t{};
        }

        [[nodiscard]] static constexpr auto memberNames() noexcept
        {
            return std::apply([](auto... member) { return std::array{ member.name()... }; }, members() );
        }

        [[nodiscard]] static constexpr int index(const std::string_view& memberName )
        {
            constexpr auto names = memberNames();
            const auto iBegin = std::begin(names);
            const auto iEnd = std::end(names);
            auto iCursor = iBegin;
            while (iCursor != iEnd && *iCursor != memberName) { ++iCursor; }
            if (iCursor == iEnd)
                throw std::invalid_argument("cppReflect17: Member 'name' is not bound for type");
            return std::distance(iBegin, iCursor);
        }

        [[nodiscard]] static constexpr auto mirror() noexcept
        {
            return typename internal::member_meta<T>::mirror{};
        }

        [[nodiscard]] static constexpr auto mirror(const type_t& instance) noexcept
        {
            return std::apply([&instance](auto... member) -> typename internal::member_meta<T>::mirror_const_tie 
                { return { member.tie(instance)... }; }, members());
        }

        [[nodiscard]] static constexpr auto mirror( type_t& instance) noexcept
        {
            return std::apply([&instance](auto... member) -> typename internal::member_meta<T>::mirror_tie
                { return { member.tie(instance)... }; }, members());
        }
#if 1
        [[nodiscard]] static constexpr auto ref(const type_t& instance) noexcept
        {
            return std::apply([&instance](auto... member) -> typename internal::member_meta<T>::mirror_const_ref
                { return { *member.tie(instance)... }; }, members());
        }

       // [[nodiscard]] static constexpr auto mirror(type_t& instance) noexcept
       // {
       //     return std::apply([&instance](auto... member) -> typename internal::member_meta<T>::mirror_tie
       //         { return { member.tie(instance)... }; }, members());
       // }
#endif

        [[nodiscard]] static constexpr auto tie(const type_t& instance) noexcept
        {
            return cppReflect17::tie{ instance };
        }

        [[nodiscard]] static constexpr auto tie(type_t& instance) noexcept
        {
            return cppReflect17::tie{ instance };
        }

    public: //< Instance functions

        [[nodiscard]] constexpr auto operator()( const type_t& instance ) const
        {
            return tie( instance );
        }

        [[nodiscard]] constexpr auto operator()(type_t& instance) const
        {
            return tie(instance);
        }

    };

    template <typename T>
    constexpr bool hasExplicitReflection() noexcept
    {
        return internal::member_meta<T>::hasExplicit;
    }


#if 0 //, @todo free functions for short interface...?
    template <typename T>
    constexpr std::string_view name() noexcept
    {
        return reflect<T>::name();
    }

    template <typename T>
    constexpr auto members() noexcept
    {
        return reflect<T>::members();
    }
#endif
    

 } ///END: cppReflect17