cmake_minimum_required(VERSION 3.18)

project(sub0reflect)

add_library( sub0reflect INTERFACE )
add_library( sub0reflect::sub0reflect ALIAS sub0reflect )

target_compile_features( sub0reflect 
    INTERFACE 
        cxx_std_17 
)

target_include_directories(sub0reflect 
    INTERFACE 
        ".." 
        #$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        #$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources( sub0reflect 
    INTERFACE 
        "sub0reflect/sub0reflect.hpp"
        "sub0reflect/arity.hpp"
        "sub0reflect/as_tuple.hpp"
        "sub0reflect/preprocessor.hpp"
        "sub0reflect/static_assert_equal.hpp"
)