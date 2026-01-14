juce_add_binary_data(bin_data SOURCES 
    ModelData/cnn_contour_model.json
    ModelData/cnn_note_model.json
    ModelData/cnn_onset_1_model.json
    ModelData/cnn_onset_2_model.json
    ModelData/features_model.ort
)

message("erik1 ${RTNeural_SOURCE_DIR}")

add_library(BasicPitchCNN STATIC 
    Model/BasicPitchCNN.cpp
    Model/BasicPitch.cpp
    Model/Features.cpp
    Model/Notes.cpp
)

target_include_directories(BasicPitchCNN PUBLIC 
    ${RTNeural_SOURCE_DIR} 
    ${onnx_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/utilities/dsp/pitch/Model
    
    )
target_link_libraries(BasicPitchCNN PUBLIC bin_data ${onnx_SOURCE_DIR}/lib/onnxruntime.lib)
