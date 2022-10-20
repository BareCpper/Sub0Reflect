# Sets the minimum version of CMake required to build the native library.
# @note 3.20 required for `GENERATED` attribute to be project-wide i.e. Version.h
cmake_minimum_required(VERSION 3.20)

# Provide path for script modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
find_package(Version.cmake)

# Detect if Sub0Reflect is being included inside another project
# - We can disable test builds by default etc.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(SUB0REFLECT_ISNOT_SUBPROJECT FALSE)
else()
  set(SUB0REFLECT_ISNOT_SUBPROJECT TRUE)
endif()

project(Sub0Reflect VERSION 0.1.2 LANGUAGES CXX )

# Provide path for scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

include(GNUInstallDirs)
option(SUB0REFLECT_BUILD_TESTING "Build unit-tests" ${SUB0REFLECT_ISNOT_SUBPROJECT})
option(SUB0REFLECT_BUILD_EXAMPLES "Build examples" ${SUB0REFLECT_ISNOT_SUBPROJECT})
#option(SUB0REFLECT_ENABLE_COVERAGE "Generate coverage for unit-tests" OFF)
#option(SUB0REFLECT_ENABLE_WERROR "Enable all warnings as errors" ON)
#option(SUB0REFLECT_INSTALL_DOCS "Install documentation alongside library" ON)

# Define some folders
# @todo Organise targets into folders i.e Tests etc
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(SUB0REFLECT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(SUB0REFLECT_UNITTEST_DIRECTORY ${SUB0REFLECT_DIRECTORY}/tests)
set(SUB0REFLECT_BENCHMARK_DIRECTORY ${SUB0REFLECT_DIRECTORY}/benchmark)
set(SUB0REFLECT_INCLUDE_DIRECTORY ${SUB0REFLECT_DIRECTORY}/include)

add_subdirectory(include)

if ( ${SUB0REFLECT_BUILD_TESTING} )
    enable_testing()
    
    #add_custom_target( sub0reflect_testBuild INTERFACE ALL )
    #target_link_libraries(sub0reflect_testBuild
    #    INTERFACE
    #        sub0reflect::sub0reflect
    #)
    #add_dependencies(sub0reflect_tests
    #        sub0reflect::tests
    #)
endif()

add_subdirectory(tests)
if ( NOT SUB0REFLECT_BUILD_TESTING)
    set_property(DIRECTORY tests PROPERTY EXCLUDE_FROM_ALL)
endif()

add_subdirectory(examples)
if ( NOT SUB0REFLECT_BUILD_EXAMPLES)
    set_property(DIRECTORY examples PROPERTY EXCLUDE_FROM_ALL)
endif()

# Install project if not included by add_subdirecrory(Sub0Reflect) from another project
#if (SUB0REFLECT_ISNOT_SUBPROJECT)
#
#    include(CMakePackageConfigHelpers)
#    
#    set(SUB0REFLECT_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Sub0Reflect")
#    
#    configure_package_config_file(
#        "${CMAKE_CURRENT_LIST_DIR}/cmake/Sub0ReflectConfig.cmake.in"
#        "${CMAKE_CURRENT_BINARY_DIR}/Sub0ReflectConfig.cmake"
#        INSTALL_DESTINATION
#          "${SUB0REFLECT_CMAKE_CONFIG_DESTINATION}"
#    )
#
#    write_basic_package_version_file(
#        "${PROJECT_BINARY_DIR}/Sub0ReflectConfigVersion.cmake"
#        COMPATIBILITY 
#            SameMajorVersion
#    )
#
#    # Install Sub0Reflect target as Sub0Reflect::Sub0Reflect
#    # @todo Test/verify install
#    install(
#        TARGETS
#            sub0reflect
#        EXPORT
#            Sub0ReflectTargets
#        DESTINATION 
#            ${CMAKE_INSTALL_LIBDIR}
#            
#            
#        #RUNTIME DESTINATION bin ${CMAKE_INSTALL_BINDIR}
#        #LIBRARY DESTINATION lib ${CMAKE_INSTALL_LIBDIR}
#        #ARCHIVE DESTINATION lib ${CMAKE_INSTALL_LIBDIR} # should add lib
#        
#        #LIBRARY DESTINATION lib COMPONENT Runtime
#        #ARCHIVE DESTINATION lib COMPONENT Development
#        #RUNTIME DESTINATION bin COMPONENT Runtime
#        #PUBLIC_HEADER DESTINATION include COMPONENT Development
#        #BUNDLE DESTINATION bin COMPONENT Runtime
#    )
#    
#    # Install targets
#    # @todo Gain understanding for what this is doing - is it generating cmake for the targets?
#    install(
#        EXPORT
#            Sub0ReflectTargets
#        NAMESPACE
#            Sub0::Reflect
#        DESTINATION
#            ${SUB0REFLECT_CMAKE_CONFIG_DESTINATION}
#    )
#    
#    # Install version and config    
#    install(
#        FILES 
#            "${PROJECT_BINARY_DIR}/Sub0ReflectConfigVersion.cmake"
#            "${PROJECT_BINARY_DIR}/Sub0ReflectConfig.cmake"
#        DESTINATION 
#            "${SUB0REFLECT_CMAKE_CONFIG_DESTINATION}"
#    )
#    
#    # Install include sources
#    install(
#        DIRECTORY
#            "include/"
#        DESTINATION
#            "${CMAKE_INSTALL_INCLUDEDIR}"
#    )
#    
#endif()