cmake_minimum_required(VERSION 3.22)

project("Abyss Lounge" VERSION 0.1)

enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEPENDENCY_DIR ${CMAKE_CURRENT_LIST_DIR}/external)
include_directories(${CMAKE_CURRENT_LIST_DIR}/utilities)
include(FetchContent)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(WebView)
include(JUCE)
include(Pluginval)
include(Doxygen)
include(Installer)

if(NOT FETCHCONTENT_FULLY_DISCONNECTED)
    include(NAM)
    add_subdirectory(utilities)
    add_subdirectory(resources)
    add_subdirectory(EQ)
    add_subdirectory(Synth)
    add_subdirectory(Abyss)
    add_subdirectory(Black_Lounge)
    add_subdirectory(Abyss_Drums)
endif()