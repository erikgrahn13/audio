cmake_minimum_required(VERSION 3.15)

add_library(Filter STATIC
    ${CMAKE_CURRENT_LIST_DIR}/Filter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Biquad.cpp
)
target_compile_options(Filter PRIVATE -fPIC)
target_include_directories(Filter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

add_library(NoiseReduction STATIC
    ${CMAKE_CURRENT_LIST_DIR}/NoiseReduction.cpp
)
target_link_libraries(NoiseReduction PRIVATE
    juce::juce_dsp
)
target_compile_options(NoiseReduction PRIVATE -fPIC)
target_include_directories(NoiseReduction PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

# add_library(Biquad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/Biquad.cpp)
# target_compile_options(Biquad PRIVATE -fPIC)
# target_include_directories(Biquad PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR})