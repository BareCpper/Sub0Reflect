cmake_minimum_required(VERSION 3.18)

project(cppReflect17)

add_library( cppReflect17_impl INTERFACE )
add_library( cppReflect17::impl ALIAS cppReflect17_impl )

target_compile_features( cppReflect17_impl INTERFACE cxx_std_17 )
target_include_directories(cppReflect17_impl INTERFACE ".." )

target_sources( cppReflect17_impl 
INTERFACE 
        "cppReflect17.hpp"
        "arity.hpp"
        "as_tuple.hpp"
        "preprocessor.hpp"
        "static_assert_equal.hpp"
)

add_subdirectory(test)


add_library( cppReflect17 INTERFACE )
target_link_libraries(cppReflect17
    INTERFACE
        cppReflect17::impl
)
add_dependencies(cppReflect17
        cppReflect17::tests
)