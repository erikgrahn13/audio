FetchContent_Declare(
    nam
    URL https://github.com/sdatkinson/NeuralAmpModelerCore/archive/refs/tags/v0.2.0.zip
    SOURCE_DIR ${DEPENDENCY_DIR}/nam
    SOURCE_SUBDIR IGNORE
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(nam)

# eigen and nlohmann-json are dependencies that nam requires under the hood
FetchContent_Declare(
    eigen
    URL https://gitlab.com/libeigen/eigen/-/archive/87300c93cae6a8afd9a4f8aa8d9d5c5324cf02e1/eigen-87300c93cae6a8afd9a4f8aa8d9d5c5324cf02e1.zip
    SOURCE_SUBDIR IGNORE
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(eigen)

FetchContent_Declare(
    nlohmann-json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
    SOURCE_SUBDIR IGNORE
    DOWNLOAD_NO_EXTRACT TRUE
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(nlohmann-json)

set(DEPENDENCY_FILES
    ${nam_SOURCE_DIR}/NAM/get_dsp.cpp
    ${nam_SOURCE_DIR}/NAM/dsp.cpp
    ${nam_SOURCE_DIR}/NAM/lstm.cpp
    ${nam_SOURCE_DIR}/NAM/convnet.cpp
    ${nam_SOURCE_DIR}/NAM/wavenet.cpp
    ${nam_SOURCE_DIR}/NAM/util.cpp
    ${nam_SOURCE_DIR}/NAM/activations.cpp
)

add_library(nam_dependency STATIC ${DEPENDENCY_FILES})
target_compile_features(nam_dependency PUBLIC cxx_std_23)
set_target_properties(nam_dependency PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(nam_dependency
    PUBLIC
    NAM_SAMPLE_FLOAT
    DSP_SAMPLE_FLOAT
)

target_include_directories(nam_dependency SYSTEM PUBLIC
    ${nam_SOURCE_DIR}
    ${eigen_SOURCE_DIR}
    ${nlohmann-json_SOURCE_DIR}
)
