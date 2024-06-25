add_compile_definitions(JUCE_DISPLAY_SPLASH_SCREEN=0)
FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.0
  SOURCE_DIR ${DEPENDENCY_DIR}/juce
)

FetchContent_MakeAvailable(juce)
