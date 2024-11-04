FetchContent_Declare(
  asiosdk
  URL https://www.steinberg.net/asiosdk
  SOURCE_DIR ${DEPENDENCY_DIR}/asiosdk
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(asiosdk)

include_directories(${asiosdk_SOURCE_DIR}/common)