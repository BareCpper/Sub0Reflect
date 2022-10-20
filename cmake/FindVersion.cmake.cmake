cmake_minimum_required(VERSION 3.18) #FetchContent

include(CPM)
CPMAddPackage("gh:BareCpper/Version.cmake#master")

if ( NOT VERSION_SET )
    message( FATAL_ERROR "Version.cmake is required")
endif()