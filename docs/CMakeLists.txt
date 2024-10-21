find_package(Doxygen)

include (FetchContent)

    FetchContent_Declare(
        doxygen-awesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
        GIT_TAG v2.3.3
    )
    FetchContent_MakeAvailable(doxygen-awesome)

    set(DOXYGEN_WARN_AS_ERRORS YES)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ../README.md)
    set(DOXYGEN_EXCLUDE_PATTERNS */external/* *build/*)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome.css
                                    ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-sidebar-only.css
                                    ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-sidebar-only-darkmode-toggle.css
                                    custom.css)

    set(DOXYGEN_HTML_HEADER header.html)
    set(DOXYGEN_HTML_EXTRA_FILES ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
                                ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
                                ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js)

    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_COLORSTYLE LIGHT)
    set(DOXYGEN_PROJECT_NAME "FahlGrahn Audio")
    set(DOXYGEN_PROJECT_NUMBER "1.0.0")


doxygen_add_docs(
    docs
    ../
    COMMENT "Generate documentation"
)