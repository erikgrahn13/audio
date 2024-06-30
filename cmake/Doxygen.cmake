if(WIN32)
    set(DOXYGEN_URL "https://github.com/doxygen/doxygen/releases/download/Release_1_11_0/doxygen-1.11.0.windows.x64.bin.zip")
    set(DOXYGEN_BINARY_PATH "doxygen.exe")
elseif(APPLE)
    set(DOXYGEN_URL "https://github.com/doxygen/doxygen/releases/download/Release_1_11_0/Doxygen-1.11.0.dmg")
    set(DOXYGEN_BINARY_PATH "doxygen-1.11.0/bin/doxygen")
elseif(UNIX)
    set(DOXYGEN_URL "https://github.com/doxygen/doxygen/releases/download/Release_1_11_0/doxygen-1.11.0.linux.x86_64.bin.tar.gz")
    set(DOXYGEN_BINARY_PATH "doxygen-1.11.0/bin/doxygen")
endif()

FetchContent_Declare(
  doxygen
  URL ${DOXYGEN_URL}
)
FetchContent_MakeAvailable(doxygen)

FetchContent_Declare(
  doxygen-awesome
  GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
  GIT_TAG v2.3.3
)

FetchContent_MakeAvailable(doxygen-awesome)

set(DOXYGEN_EXECUTABLE ${doxygen_SOURCE_DIR}/${DOXYGEN_BINARY_PATH})
include(FindDoxygen)

set(DOXYGEN_USE_MDFILE_AS_MAINPAGE YES)
set(DOXYGEN_EXCLUDE_PATTERNS */external/* *build/*)
set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome.css
                                  ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-sidebar-only.css
                                  ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-sidebar-only-darkmode-toggle.css
                                  ${CMAKE_SOURCE_DIR}/docs/custom.css)

set(DOXYGEN_HTML_HEADER ${CMAKE_SOURCE_DIR}/docs/header.html)
set(DOXYGEN_HTML_EXTRA_FILES ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
                             ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
                             ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js)

set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_HTML_COLORSTYLE LIGHT)


doxygen_add_docs(
    docs
    ${PROJECT_SOURCE_DIR}
    COMMENT "Generate documentation"
)