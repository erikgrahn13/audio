


if(WIN32)
  set(JUCE_URL "https://github.com/juce-framework/JUCE/releases/download/8.0.2/juce-8.0.2-windows.zip")
elseif(APPLE)
  set(JUCE_URL "https://github.com/juce-framework/JUCE/releases/download/8.0.2/juce-8.0.2-osx.zip")
elseif(UNIX)
  set(JUCE_URL "https://github.com/juce-framework/JUCE/releases/download/8.0.2/juce-8.0.2-linux.zip")
else()
  message(FATAL_ERROR "Not supported platform")
endif()

FetchContent_Declare(
  juce
  URL ${JUCE_URL}
  SOURCE_DIR ${DEPENDENCY_DIR}/juce
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(juce)