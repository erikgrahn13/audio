cmake_minimum_required(VERSION 3.22)

project(Audio)

enable_testing()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEPENDENCY_DIR ${CMAKE_CURRENT_LIST_DIR}/external)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(JUCE)
include(Pluginval)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

juce_add_binary_data(Fonts HEADER_NAME "Fonts.h" SOURCES resources/fonts/ArtDystopia.ttf
                                                                  resources/fonts/fontaudio.ttf )

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.7" CACHE STRING "Minimum OS X deployment version" FORCE)
add_subdirectory(utilities)
add_subdirectory(EQ)
