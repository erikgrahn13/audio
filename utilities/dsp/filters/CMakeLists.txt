cmake_minimum_required(VERSION 3.15)

add_library(Filter STATIC
     ${CMAKE_CURRENT_LIST_DIR}/Filter.cpp
     ${CMAKE_CURRENT_LIST_DIR}/Biquad.cpp
)
target_compile_options(Filter PRIVATE -fPIC)
target_include_directories(Filter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

# add_library(Biquad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Biquad.cpp)
# target_compile_options(Biquad PRIVATE -fPIC)
# target_include_directories(Biquad PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR})