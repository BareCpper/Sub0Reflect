# Provide some groupings for IDEs
#SOURCE_GROUP("benchmark" FILES ${BENCHMARK_HEADERS} ${BENCHMARK_SOURCES})
#SOURCE_GROUP("Tests" FILES ${TEST_SOURCES})

find_package(GTest)

#add_executable( Sub0_Reflect_SelfTest )
#add_executable( Sub0::Reflect::SelfTest ALIAS Sub0_Reflect_SelfTest )
#
#target_sources( Sub0_Reflect_SelfTest
#    PRIVATE
#        "${CMAKE_CURRENT_LIST_DIR}/test_types.hpp"  
#        "${CMAKE_CURRENT_LIST_DIR}/test_constexpr.cpp"  
#)
#target_link_libraries(Sub0_Reflect_SelfTest
#    PUBLIC
#        Sub0::Reflect
#        GTest::Main
#)
#target_compile_features(Sub0_Reflect_SelfTest PRIVATE cxx_std_17)
#
#
#gtest_discover_tests( Sub0_Reflect_SelfTest )

add_executable( sub0reflect_tests )
add_executable( sub0reflect::tests ALIAS sub0reflect_tests )

target_compile_features( sub0reflect_tests PUBLIC cxx_std_17 )
target_sources( sub0reflect_tests
    PRIVATE
    "sub0reflect_constexpr_test.cpp")

find_package(GoogleTest)
target_link_libraries(sub0reflect_tests
    PUBLIC
        sub0reflect::sub0reflect
        GTest::Main
)
gtest_discover_tests( sub0reflect_tests )