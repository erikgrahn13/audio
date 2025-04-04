FetchContent_Declare(
    nam
    GIT_REPOSITORY https://github.com/sdatkinson/NeuralAmpModelerCore.git
    GIT_TAG v0.2.0
    SOURCE_DIR ${DEPENDENCY_DIR}/nam
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_Populate(
    nam
)

FetchContent_Declare(
    nlohmann-json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
    SOURCE_DIR ${DEPENDENCY_DIR}/nlohmann-json
    DOWNLOAD_NO_EXTRACT TRUE
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

set_target_properties(nam_dependency PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(nam_dependency
    PUBLIC
    NAM_SAMPLE_FLOAT
    DSP_SAMPLE_FLOAT
)

target_include_directories(nam_dependency SYSTEM PUBLIC
    ${nam_SOURCE_DIR}
    ${nam_SOURCE_DIR}/Dependencies/eigen
    ${nlohmann-json_SOURCE_DIR}
)
