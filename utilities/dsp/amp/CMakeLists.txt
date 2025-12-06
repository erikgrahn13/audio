cmake_minimum_required(VERSION 3.19)

add_library(Amp STATIC
    Amp.cpp
    Amp.h
)

target_link_libraries(Amp PUBLIC
    nam_dependency
)

target_compile_options(Amp PRIVATE -fPIC)

target_include_directories(Amp PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)