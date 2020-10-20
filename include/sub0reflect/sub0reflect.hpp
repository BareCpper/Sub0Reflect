/** Sub0Reflect core header-only library
 * @remark Simple and Lightweight Parser-less C++17 Reflection at Compile-time - Programmatically describe simple struct memory-layout or interface-definition
 * 
 *  This file is part of Sub0Pub. Original project source available at https://github.com/Crog/Sub0Pub/blob/master/sub0pub.hpp
 * 
 *  MIT License
 *
 * Copyright (c) 2020 Craig Hutchinson <craig-sub0reflect@crog.uk>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files 
 *  (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, 
 *  publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do 
 *  so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 *  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef CROG_SUB0BUS_HPP
#define CROG_SUB0BUS_HPP

//#include <algorithm>
//#include <cassert> //< assert
//#include <cstring> //< std::strcmp
#include <array> //< std::array
#include <tuple> //< std::tie
#include <functional> //< std::mem_fn

 /// @todo 0 vs nullptr C++11 only
#if 1 /// @todo cstdint not always available ... C++11/C99 only 
    #include <cstdint> //< uint32_t
#else
    typedef unsigned char uint8_t;
    typedef unsigned int uint32_t;
#endif

#ifndef SUB0_EXPERIMENTAL
#define SUB0_EXPERIMENTAL false ///< Experimental functionality that may be later removed/dropped
#endif


/** Sub0Pub top-level namespace
*/
namespace sub0
{
    template <typename T, T> struct MemberTie;

    template <typename Type_t, typename Owner_t, Type_t(Owner_t:: * MemPtr)>
    struct MemberTie< Type_t(Owner_t::*), MemPtr>
    {
        using type_t = Type_t;
        using owner_t = Owner_t;

        /** Convenience member function @see sub0::tie(MemberTie) */
        constexpr type_t& operator()(Owner_t& instance) noexcept
        {
            return sub0::tie(instance, *this);
        }

        constexpr const type_t& operator()(const Owner_t& instance) noexcept
        {
            return sub0::tie(instance, *this);
        }

        /** Convenience member function @see sub0::name(MemberTie) */
        constexpr std::string_view name() const noexcept
        {
            return sub0::name(*this);
        }

    };

    template <typename T, T> struct InstanceMemberTie;

    template <typename Type_t, typename Owner_t, Type_t(Owner_t:: * MemPtr)>
    struct InstanceMemberTie< Type_t(Owner_t::*), MemPtr> : MemberTie< Type_t(Owner_t::*), MemPtr>
    {
        Owner_t& instance;

        InstanceMemberTie(Owner_t& instance) : instance(instance) {}

        operator Type_t& ()
        {
            return instance.*MemPtr;
        }

        Type_t& operator*()
        {
            return instance.*MemPtr;
        }

        operator const Type_t& () const
        {
            return instance.*MemPtr;
        }

        const Type_t& operator*() const
        {
            return instance.*MemPtr;
        }
    };

    template <typename T, T> struct ConstInstanceMemberTie;
    template <typename Type_t, typename Owner_t, Type_t(Owner_t:: * MemPtr)>
    struct ConstInstanceMemberTie< Type_t(Owner_t::*), MemPtr> : MemberTie< Type_t(Owner_t::*), MemPtr>
    {
        const Owner_t& instance;

        ConstInstanceMemberTie(const Owner_t& instance) : instance(instance) {}

        operator const Type_t& () const
        {
            return instance.*MemPtr;
        }

        const Type_t& operator*() const
        {
            return instance.*MemPtr;
        }
    };

    ///sub0::tie - tie a MemberTie to an instance of Owner_t type
    template<typename Type_t, typename Owner_t, Type_t(Owner_t:: * MemPtr)>
    constexpr auto tie(Owner_t& instance, MemberTie< Type_t(Owner_t::*), MemPtr>) noexcept
    {
        return InstanceMemberTie< Type_t(Owner_t::*), MemPtr>(instance);
    }

    ///sub0::tie - tie a MemberTie to an instance of Owner_t type
    template<typename Type_t, typename Owner_t, Type_t(Owner_t:: * MemPtr)>
    constexpr auto tie(const Owner_t& instance, MemberTie< Type_t(Owner_t::*), MemPtr>) noexcept
    {
        return ConstInstanceMemberTie< Type_t(Owner_t::*), MemPtr>(instance);
    }

    ///sub0::tie - tie a tuple-list of MemberTie to an instance of Owner_t type
    template<typename Owner_t, typename... MemberTies>
    constexpr decltype(auto) tie(Owner_t& instance, std::tuple<MemberTies...> members) noexcept
    {
        return std::apply([&instance](MemberTies const&... members) {
            return std::make_tuple(tie(instance, members)...); }, members);
    }

    ///sub0::tie - tie a tuple-list of MemberTie to an instance of Owner_t type
    template<typename Owner_t, typename... MemberTies>
    constexpr decltype(auto) tie(const Owner_t& instance, std::tuple<MemberTies...> members) noexcept
    {
        return std::apply([&instance](MemberTies const&... members) {
            return std::make_tuple(tie(instance, members)...); }, members);
    }

    ///sub0::tie - tie a MemberTie to an instance of Owner_t type
    template <typename MemberTie_t>
    constexpr typename MemberTie_t::type_t& tie(typename MemberTie_t::owner_t& instance) noexcept
    {
        return MemberTie_t()(instance);
    }

    ///sub0::tie - tie a MemberTie to an instance of Owner_t type
    template <typename MemberTie_t>
    constexpr typename const MemberTie_t::type_t& tie(typename const MemberTie_t::owner_t& instance) noexcept
    {
        return MemberTie_t()(instance);
    }

    template<typename Type_t, typename Owner_t, Type_t(Owner_t:: * MemPtr)>
    constexpr std::string_view name(MemberTie< Type_t(Owner_t::*), MemPtr>) noexcept
    {
        return reflect(Owner_t()).name(MemberTie< Type_t(Owner_t::*), MemPtr>());
    }

    template<typename Owner_t>
    constexpr auto reflect()
    {
        return reflect(Owner_t());
    }
        
} // END: sub0

#endif
