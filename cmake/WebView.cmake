function(enable_webview)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(WEBVIEW_FILES_ZIP_NAME "webview_files.zip")
        set(TARGET_WEBVIEW_FILES_ZIP_PATH "${CMAKE_CURRENT_BINARY_DIR}/${WEBVIEW_FILES_ZIP_NAME}")

        add_custom_target(${PROJECT_NAME}ZipWebViewFiles
            COMMAND
            ${CMAKE_COMMAND} -E tar cvf
            "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
            --format=zip
            "${CMAKE_CURRENT_SOURCE_DIR}/ui/dist"
            BYPRODUCTS
            "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
            WORKING_DIRECTORY
            "${CMAKE_CURRENT_SOURCE_DIR}/ui"
            COMMENT "Zipping WebView files..."
            VERBATIM
        )

        target_compile_definitions(${PROJECT_NAME} PRIVATE ZIPPED_FILES_PREFIX="dist/")

        juce_add_binary_data(${PROJECT_NAME}WebViewFiles
            HEADER_NAME WebViewFiles.h
            NAMESPACE webview_files
            SOURCES ${TARGET_WEBVIEW_FILES_ZIP_PATH}
        )

        add_dependencies(${PROJECT_NAME}WebViewFiles ${PROJECT_NAME}ZipWebViewFiles)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}WebViewFiles)
    endif()

    target_compile_definitions(${PROJECT_NAME} PUBLIC
        JUCE_WEB_BROWSER=1
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
    )
    target_link_libraries(${PROJECT_NAME} PUBLIC juce::juce_gui_extra)
endfunction()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT FETCHCONTENT_FULLY_DISCONNECTED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.1)
    include_directories(${GTK3_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS})
endif()

if(WIN32)
    FetchContent_Declare(
        wil
        URL https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.211019.2
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    # Microsoft.Web.WebView2
    FetchContent_Declare(
        webview2
        URL https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.1072.54
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/Microsoft.Web.WebView2
    )

    FetchContent_MakeAvailable(wil webview2)

    set(JUCE_WEBVIEW2_PACKAGE_LOCATION "${CMAKE_BINARY_DIR}/_deps" CACHE PATH "Path to WebView2 package" FORCE)
endif()