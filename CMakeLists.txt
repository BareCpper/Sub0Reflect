# @note 3.7.1 as version use in WIndows Git-Bash
# @todo >3.10 needed for include(GoogleTest)?
cmake_minimum_required(VERSION 3.7.1)

# Detect if Sub0buS is being included inside another project
# - We can disable test generation etc.
if(DEFINED PROJECT_NAME)
  set(IS_SUBPROJECT ON)
endif()

project(Sub0buS VERSION 0.1.2 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 98)

# Provide path for scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

#include(GNUInstallDirs)

# @todo Should set cmake 3.10
#include(GoogleTest)

#option(SUB0BUS_USE_VALGRIND "Perform SelfTests with Valgrind" OFF)
option(SUB0BUS_BUILD_TESTING "Build unit-tests" ON)
option(SUB0BUS_BUILD_EXAMPLES "Build examples" ON)
#option(SUB0BUS_ENABLE_COVERAGE "Generate coverage for unit-tests" OFF)
#option(SUB0BUS_ENABLE_WERROR "Enable all warnings as errors" ON)
#option(SUB0BUS_INSTALL_DOCS "Install documentation alongside library" ON)

# Define some folders
# @todo Organise targets into folders i.e Tests etc
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(SUB0BUS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(UNITTEST_DIRECTORY ${SUB0BUS_DIRECTORY}/test)
set(BENCHMARK_DIRECTORY ${SUB0BUS_DIRECTORY}/benchmark)
set(INCLUDE_DIRECTORY ${SUB0BUS_DIRECTORY}/include)

if (BUILD_TESTING AND SUB0BUS_BUILD_TESTING AND NOT_SUBPROJECT)
    add_subdirectory(projects)
endif()

if(SUB0BUS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Sub0buS as header only target
# + Namespaced alias for linking against core library from client
add_library(Sub0buS INTERFACE)
add_library(Sub0buS::Sub0buS ALIAS Sub0buS)

target_include_directories(Sub0buS
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources( Sub0buS 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/sub0bus/sub0bus.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/sub0bus/sub0bus.hpp>
)

# Install project if not included by add_subdirecrory(Sub0buS) from another project
if (NOT IS_SUBPROJECT)

    include(CMakePackageConfigHelpers)
    
    set(SUB0BUS_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Sub0buS")
    
    configure_package_config_file(
        "${CMAKE_CURRENT_LIST_DIR}/cmake/Sub0buSConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Sub0buSConfig.cmake"
        INSTALL_DESTINATION
          "${SUB0BUS_CMAKE_CONFIG_DESTINATION}"
    )

    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/Sub0buSConfigVersion.cmake"
        COMPATIBILITY 
            SameMajorVersion
    )

    # Install Sub0buS target as Sub0buS::Sub0buS
    # @todo Test/verify install
    install(
        TARGETS
            Sub0buS
        EXPORT
            Sub0buSTargets
        DESTINATION 
            ${CMAKE_INSTALL_LIBDIR}
            
            
        #RUNTIME DESTINATION bin ${CMAKE_INSTALL_BINDIR}
        #LIBRARY DESTINATION lib ${CMAKE_INSTALL_LIBDIR}
        #ARCHIVE DESTINATION lib ${CMAKE_INSTALL_LIBDIR} # should add lib
        
        #LIBRARY DESTINATION lib COMPONENT Runtime
        #ARCHIVE DESTINATION lib COMPONENT Development
        #RUNTIME DESTINATION bin COMPONENT Runtime
        #PUBLIC_HEADER DESTINATION include COMPONENT Development
        #BUNDLE DESTINATION bin COMPONENT Runtime
    )
    
    # Install targets
    # @todo Gain understanding for what this is doing - is it generating cmake for the targets?
    install(
        EXPORT
            Sub0buSTargets
        NAMESPACE
            Sub0buS::
        DESTINATION
            ${SUB0BUS_CMAKE_CONFIG_DESTINATION}
    )
    
    # Install version and config    
    install(
        FILES 
            "${PROJECT_BINARY_DIR}/Sub0buSConfigVersion.cmake"
            "${PROJECT_BINARY_DIR}/Sub0buSConfig.cmake"
        DESTINATION 
            "${SUB0BUS_CMAKE_CONFIG_DESTINATION}"
    )
    
    # Install include sources
    install(
        DIRECTORY
            "include/"
        DESTINATION
            "${CMAKE_INSTALL_INCLUDEDIR}"
    )
    
endif()