add_compile_definitions(JUCE_DISPLAY_SPLASH_SCREEN=0)
FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 7.0.12
  SOURCE_DIR ${DEPENDENCY_DIR}/juce
)

set(JUCE_BUILD_EXTRAS ON CACHE INTERNAL "")
set(JUCE_BUILD_EXAMPLES ON CACHE INTERNAL "")
FetchContent_MakeAvailable(juce)
