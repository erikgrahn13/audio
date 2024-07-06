cmake_minimum_required(VERSION 3.15)

add_library(Biquad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Biquad.cpp)
target_compile_options(Biquad PRIVATE -fPIC)
target_include_directories(Biquad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})