function(enable_webview)
    file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/ui")

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/ui/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/ui/")
        file(COPY "${juce_SOURCE_DIR}/modules/juce_gui_extra/native/javascript/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/ui/juce/")
        file(COPY "${CMAKE_SOURCE_DIR}/widgets/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/ui/widgets/")
        file(COPY "${CMAKE_SOURCE_DIR}/resources/fonts/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/ui/fonts/")

        set(WEBVIEW_FILES_ZIP_NAME "webview_files.zip")
        set(TARGET_WEBVIEW_FILES_ZIP_PATH "${CMAKE_CURRENT_BINARY_DIR}/${WEBVIEW_FILES_ZIP_NAME}")

        add_custom_target(${PROJECT_NAME}ZipWebViewFiles
            COMMAND
            ${CMAKE_COMMAND} -E tar cvf
            "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
            --format=zip
            "${CMAKE_CURRENT_BINARY_DIR}/ui"
            BYPRODUCTS
            "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
            WORKING_DIRECTORY
            "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Zipping WebView files..."
            VERBATIM
        )

        target_compile_definitions(${PROJECT_NAME} PRIVATE ZIPPED_FILES_PREFIX="ui/")

        juce_add_binary_data(${PROJECT_NAME}WebViewFiles
            HEADER_NAME WebViewFiles.h
            NAMESPACE webview_files
            SOURCES ${TARGET_WEBVIEW_FILES_ZIP_PATH}
        )

        add_dependencies(${PROJECT_NAME}WebViewFiles ${PROJECT_NAME}ZipWebViewFiles)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}WebViewFiles)

    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(LINK_DEST "${CMAKE_CURRENT_BINARY_DIR}/ui")
        set(SRC_UI "${CMAKE_CURRENT_SOURCE_DIR}/ui")
        set(SRC_WIDGETS "${CMAKE_SOURCE_DIR}/widgets")
        set(SRC_JUCE_JS "${juce_SOURCE_DIR}/modules/juce_gui_extra/native/javascript")

        add_custom_command(
            OUTPUT "${LINK_DEST}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${LINK_DEST}"
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_UI}/index.html" "${LINK_DEST}/index.html"
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_UI}/style.css" "${LINK_DEST}/style.css"
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_UI}/resources" "${LINK_DEST}/resources"
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_UI}/index.js" "${LINK_DEST}/index.js"

            COMMENT "Symlinking UI directory"
            VERBATIM
        )

        add_custom_command(
            OUTPUT "${LINK_DEST}/widgets"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${LINK_DEST}"
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_WIDGETS}" "${LINK_DEST}/widgets"
            COMMENT "Symlinking widgets directory"
            VERBATIM
        )

        add_custom_command(
            OUTPUT "${LINK_DEST}/juce"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${LINK_DEST}"
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_JUCE_JS}" "${LINK_DEST}/juce"
            COMMENT "Symlinking JUCE JavaScript directory"
            VERBATIM
        )

        add_custom_command(
            OUTPUT "${LINK_DEST}/fonts"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${LINK_DEST}"
            COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/resources/fonts" "${LINK_DEST}/fonts"
            COMMENT "Symlinking fonts directory"
            VERBATIM
        )

        add_custom_target(${PROJECT_NAME}CreateWebviewSymlinks
            DEPENDS "${LINK_DEST}" "${LINK_DEST}/widgets" "${LINK_DEST}/juce" "${LINK_DEST}/fonts"
        )

        add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}CreateWebviewSymlinks)
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