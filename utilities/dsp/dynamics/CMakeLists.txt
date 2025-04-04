cmake_minimum_required(VERSION 3.19)

add_library(NoiseGate STATIC
    ${CMAKE_CURRENT_LIST_DIR}/NoiseGate.cpp
)

target_compile_options(NoiseGate PRIVATE -fPIC)
target_include_directories(NoiseGate PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
