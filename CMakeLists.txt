# @todo >3.10 needed for include(GoogleTest)?
cmake_minimum_required(VERSION 3.15)

# Detect if Sub0Reflect is being included inside another project
# - We can disable test generation etc.
if(DEFINED PROJECT_NAME)
  set(IS_SUBPROJECT ON)
endif()

project(Sub0Reflect VERSION 0.1.2 LANGUAGES CXX )


# Provide path for scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

#include(GNUInstallDirs)


option(SUB0BUS_BUILD_TESTING "Build unit-tests" ON)
option(SUB0BUS_BUILD_EXAMPLES "Build examples" ON)
#option(SUB0BUS_ENABLE_COVERAGE "Generate coverage for unit-tests" OFF)
#option(SUB0BUS_ENABLE_WERROR "Enable all warnings as errors" ON)
#option(SUB0BUS_INSTALL_DOCS "Install documentation alongside library" ON)

# Define some folders
# @todo Organise targets into folders i.e Tests etc
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(SUB0BUS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(UNITTEST_DIRECTORY ${SUB0BUS_DIRECTORY}/tests)
set(BENCHMARK_DIRECTORY ${SUB0BUS_DIRECTORY}/benchmark)
set(INCLUDE_DIRECTORY ${SUB0BUS_DIRECTORY}/include)

enable_testing()
add_subdirectory(tests)

add_subdirectory(examples)

# Sub0Reflect as header only target
# + Namespaced alias for linking against core library from client
add_library(Sub0Reflect INTERFACE)
add_library(Sub0::Reflect ALIAS Sub0Reflect)

target_include_directories(Sub0Reflect
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources( Sub0Reflect 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/sub0reflect/sub0reflect.hpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/sub0reflect/sub0reflect.hpp>
)

# Install project if not included by add_subdirecrory(Sub0Reflect) from another project
if (NOT IS_SUBPROJECT)

    include(CMakePackageConfigHelpers)
    
    set(SUB0BUS_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Sub0Reflect")
    
    configure_package_config_file(
        "${CMAKE_CURRENT_LIST_DIR}/cmake/Sub0ReflectConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Sub0ReflectConfig.cmake"
        INSTALL_DESTINATION
          "${SUB0BUS_CMAKE_CONFIG_DESTINATION}"
    )

    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/Sub0ReflectConfigVersion.cmake"
        COMPATIBILITY 
            SameMajorVersion
    )

    # Install Sub0Reflect target as Sub0Reflect::Sub0Reflect
    # @todo Test/verify install
    install(
        TARGETS
            Sub0Reflect
        EXPORT
            Sub0ReflectTargets
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
            Sub0ReflectTargets
        NAMESPACE
            Sub0::Reflect
        DESTINATION
            ${SUB0BUS_CMAKE_CONFIG_DESTINATION}
    )
    
    # Install version and config    
    install(
        FILES 
            "${PROJECT_BINARY_DIR}/Sub0ReflectConfigVersion.cmake"
            "${PROJECT_BINARY_DIR}/Sub0ReflectConfig.cmake"
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