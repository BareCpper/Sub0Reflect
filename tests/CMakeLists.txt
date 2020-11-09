cmake_minimum_required(VERSION 3.11)

# Provide some groupings for IDEs
#SOURCE_GROUP("benchmark" FILES ${BENCHMARK_HEADERS} ${BENCHMARK_SOURCES})
#SOURCE_GROUP("Tests" FILES ${TEST_SOURCES})


include( ${PROJECT_SOURCE_DIR}/cmake/GTest.cmake)

add_executable( Sub0_Reflect_SelfTest "" )
add_executable( Sub0::Reflect::SelfTest ALIAS Sub0_Reflect_SelfTest )

target_sources( Sub0_Reflect_SelfTest
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/test_types.hpp"  
        "${CMAKE_CURRENT_LIST_DIR}/test_constexpr.cpp"  
)
target_link_libraries(Sub0_Reflect_SelfTest
    PUBLIC
        Sub0::Reflect
        GTest::Main
)
target_compile_features(Sub0_Reflect_SelfTest PRIVATE cxx_std_17)


gtest_discover_tests( Sub0_Reflect_SelfTest )

#add_test(NAME Sub0::Reflect::SelfTest::RunTests COMMAND SelfTest)

#include(CTest)

# configure unit tests via CTest
#add_test(NAME Sub0::Reflect::SelfTest::RunTests COMMAND $<TARGET_FILE:SelfTest>)
#set_tests_properties(RunTests PROPERTIES
#    PASS_REGULAR_EXPRESSION "[0-9]+ test cases"
#    FAIL_REGULAR_EXPRESSION "Filters:"
#    COST 60
#)
