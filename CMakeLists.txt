cmake_minimum_required(VERSION 3.22)

project(Audio VERSION 0.1)

enable_testing()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEPENDENCY_DIR ${CMAKE_CURRENT_LIST_DIR}/external)
include_directories(${CMAKE_CURRENT_LIST_DIR}/utilities)
include (FetchContent)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(JUCE)
include(Pluginval)
include(Doxygen)

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.7" CACHE STRING "Minimum OS X deployment version" FORCE)

if(NOT FETCHCONTENT_FULLY_DISCONNECTED)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
add_subdirectory(utilities)
add_subdirectory(resources)
add_subdirectory(EQ)
add_subdirectory(Synth)
endif()