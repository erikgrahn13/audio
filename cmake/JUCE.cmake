

if(WIN32)
  set(JUCE_URL "https://github.com/juce-framework/JUCE/releases/download/8.0.7/juce-8.0.7-windows.zip")
elseif(APPLE)
  set(JUCE_URL "https://github.com/juce-framework/JUCE/releases/download/8.0.7/juce-8.0.7-osx.zip")
elseif(UNIX)
  set(JUCE_URL "https://github.com/juce-framework/JUCE/releases/download/8.0.7/juce-8.0.7-linux.zip")
else()
  message(FATAL_ERROR "Not supported platform")
endif()

FetchContent_Declare(
  juce
  URL ${JUCE_URL}
  SOURCE_DIR ${DEPENDENCY_DIR}/juce
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(juce)

set(CMAKE_OSX_DEPLOYMENT_TARGET "12.7" CACHE STRING "Minimum OS X deployment version" FORCE)
set_directory_properties(PROPERTIES JUCE_COMPANY_NAME ${CMAKE_PROJECT_NAME})
set(ABYSS_LOUNGE_BUNDLE_ID "com.abyss-lounge")
set(ABYSS_LOUNGE_PLUGIN_MANUFACTURER_CODE "Abys")
