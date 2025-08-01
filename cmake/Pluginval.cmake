function(enable_pluginval_testing target)
    get_target_property(bundle_id ${target} JUCE_BUNDLE_ID)

    if(NOT "${bundle_id}" MATCHES "${ABYSS_LOUNGE_BUNDLE_ID}.${target}")
        message(FATAL_ERROR "❗️ Plugin '${target}': Invalid bundle ID format. \nPlease add BUNDLE_ID \${ABYSS_LOUNGE_BUNDLE_ID}.\${PROJECT_NAME} to jude_add_plugin call for ${target}")
    endif()

    message(STATUS "✅ ${target}: ${bundle_id}")

    # Pluginval testing
    get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)
    get_target_property(PLUGIN_NAME ${PROJECT_NAME} JUCE_PRODUCT_NAME)

    add_test(NAME ${target}_VST3_TEST
        COMMAND ${pluginval_SOURCE_DIR}/${PLUGINVAL_BINARY_PATH} --strictness-level 10 --verbose --skip-gui-tests --validate-in-process ${PLUGIN_PATH}/VST3/${PLUGIN_NAME}.vst3)

    if(APPLE AND NOT JUCE_BUILD_EXTRAS)
        add_test(NAME ${target}_AU_TEST
            COMMAND ${pluginval_SOURCE_DIR}/${PLUGINVAL_BINARY_PATH} --strictness-level 10 --verbose --skip-gui-tests --validate-in-process $ENV{HOME}/Library/Audio/Plug-Ins/Components/${PLUGIN_NAME}.component)
    endif(APPLE AND NOT JUCE_BUILD_EXTRAS)
endfunction(enable_pluginval_testing)

if(WIN32)
    set(PLUGINVAL_URL "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Windows.zip")
    set(PLUGINVAL_BINARY_PATH "pluginval.exe")
elseif(APPLE)
    set(PLUGINVAL_URL "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_macOS.zip")
    set(PLUGINVAL_BINARY_PATH "Contents/MacOS/pluginval")
elseif(UNIX)
    set(PLUGINVAL_URL "https://github.com/Tracktion/pluginval/releases/latest/download/pluginval_Linux.zip")
    set(PLUGINVAL_BINARY_PATH "pluginval")
endif()

FetchContent_Declare(
    pluginval
    URL ${PLUGINVAL_URL}
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(pluginval)
