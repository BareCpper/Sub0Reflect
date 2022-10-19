#pragma once

#include <string_view>

template <typename T> constexpr std::string_view type_name();

template <>
constexpr std::string_view type_name<void>()
{ return "void"; }

namespace detail 
{
    using type_name_prober = void;

    template <typename T>
    constexpr std::string_view wrapped_type_name() 
    {
#ifdef __clang__
        return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
        return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
        return __FUNCSIG__;
#else
#   error "Unsupported compiler"
#endif
    }

    constexpr std::size_t wrapped_type_name_prefix_length() 
    { 
        return wrapped_type_name<type_name_prober>().find(type_name<type_name_prober>()); 
    }

    constexpr std::size_t wrapped_type_name_suffix_length()
     { 
        return wrapped_type_name<type_name_prober>().length() 
            - wrapped_type_name_prefix_length() 
            - type_name<type_name_prober>().length();
    }
} // END detail

template <typename T>
constexpr std::string_view type_name() 
{
    constexpr auto wrapped_name = detail::wrapped_type_name<T>();
    constexpr auto prefix_length = detail::wrapped_type_name_prefix_length();
    constexpr auto suffix_length = detail::wrapped_type_name_suffix_length();
    constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;
    return wrapped_name.substr(prefix_length, type_name_length);
}


#if 0 
namespace test 
{
    namespace s1 {
        class MyClass;
    }
}

static constexpr std::string_view tt = type_name<test::s1::MyClass>();
static constexpr std::string_view cmp = "class test::s1::MyClass"; //< @todo: MSVC has 'class' prefix
static constexpr char ta = tt[0];
static constexpr char tb = cmp[0];

//static_assert(ta == tb);
//static_assert(tt == cmp);
#endif

#if 0 //Short version...
    #include <string_view>

    template <typename T>
    constexpr auto type_name()
    {
        std::string_view name, prefix, suffix;
    #ifdef __clang__
        name = __PRETTY_FUNCTION__;
        prefix = "auto type_name() [T = ";
        suffix = "]";
    #elif defined(__GNUC__)
        name = __PRETTY_FUNCTION__;
        prefix = "constexpr auto type_name() [with T = ";
        suffix = "]";
    #elif defined(_MSC_VER)
        name = __FUNCSIG__;
        prefix = "auto __cdecl type_name<";
        suffix = ">(void)";
    #endif
        name.remove_prefix(prefix.size());
        name.remove_suffix(suffix.size());
        return name;
    }

#endif