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

add_compile_definitions(JUCE_DISPLAY_SPLASH_SCREEN=0)
set(DEPENDENCY_DIR ${CMAKE_CURRENT_LIST_DIR}/external)

# add dependencies here
CPMAddPackage(
  NAME juce
  GITHUB_REPOSITORY juce-framework/JUCE
  GIT_TAG 7.0.12
  SOURCE_DIR ${DEPENDENCY_DIR}/juce
  OPTIONS "JUCE_BUILD_EXTRAS ON" "JUCE_BUILD_EXAMPLES ON"
)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

juce_add_binary_data(Fonts HEADER_NAME "Fonts.h" SOURCES resources/fonts/ArtDystopia.ttf
                                                                  resources/fonts/fontaudio.ttf )

add_subdirectory(utilities)
add_subdirectory(EQ)
