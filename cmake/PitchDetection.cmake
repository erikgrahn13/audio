# FetchContent_Declare(
# pitch-detection
# URL https://github.com/sevagh/pitch-detection/archive/refs/tags/v2023.12.zip
# )

# FetchContent_MakeAvailable(pitch-detection)

FetchContent_Declare(
    audio-fft
    GIT_REPOSITORY https://github.com/HiFi-LoFi/AudioFFT.git
    GIT_TAG master
)

FetchContent_MakeAvailable(audio-fft)