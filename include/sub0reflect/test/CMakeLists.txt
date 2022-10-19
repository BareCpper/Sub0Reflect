

add_executable( cppReflect17_tests )
add_executable( cppReflect17::tests ALIAS cppReflect17_tests )

target_compile_features( cppReflect17_tests PUBLIC cxx_std_17 )
target_sources( cppReflect17_tests
    PRIVATE
    "cppReflect17_constexpr_test.cpp")

find_package(GoogleTest)
target_link_libraries(cppReflect17_tests
    PUBLIC
        cppReflect17::impl
        GTest::Main
)
gtest_discover_tests( cppReflect17_tests )