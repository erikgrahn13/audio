cmake_minimum_required(VERSION 3.22)

project(Audio)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

option(JUCE_BUILD_EXTRAS "Add build targets for the Projucer and other tools" ON)
option(JUCE_BUILD_EXAMPLES "Add build targets for the DemoRunner and PIPs" ON)
set(JUCE_BUILD_EXAMPLES ON CACHE BOOL "Force JUCE_BUILD_EXTRAS to ON" FORCE)

add_compile_definitions(JUCE_DISPLAY_SPLASH_SCREEN=0)

# add dependencies here
CPMAddPackage("gh:juce-framework/JUCE#7.0.12")

add_subdirectory(utilities)
add_subdirectory(EQ)
add_subdirectory(Synth)