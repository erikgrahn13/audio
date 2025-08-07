function(enable_pluginval_testing target)
    add_dependencies(${target} validator)
    get_target_property(bundle_id ${target} JUCE_BUNDLE_ID)

    if(NOT "${bundle_id}" MATCHES "${ABYSS_LOUNGE_BUNDLE_ID}.${target}")
        message(FATAL_ERROR "❗️ Plugin '${target}': Invalid bundle ID format. \nPlease add BUNDLE_ID \${ABYSS_LOUNGE_BUNDLE_ID}.\${PROJECT_NAME} to jude_add_plugin call for ${target}")
    endif()

    message(STATUS "✅ ${target}: ${bundle_id}")

    # Pluginval testing
    get_target_property(PLUGIN_NAME ${PROJECT_NAME} JUCE_PRODUCT_NAME)

    get_target_property(PLUGIN_PATH_VST3 ${target} JUCE_VST3_COPY_DIR)
    get_target_property(PLUGIN_PATH_AU ${target} JUCE_AU_COPY_DIR)
    get_target_property(PLUGIN_IS_COPIED ${target} JUCE_COPY_PLUGIN_AFTER_BUILD)

    if(NOT PLUGIN_IS_COPIED)
        if(APPLE)
            set(PLUGIN_PATH_AU "/Library/Audio/Plug-Ins/Components")

            if(CMAKE_BUILD_TYPE STREQUAL "Release")
                set(PLUGIN_PATH_VST3 "/Library/Audio/Plug-Ins/VST3")
            else()
                get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)
                set(PLUGIN_PATH_VST3 "${PLUGIN_PATH}/VST3")
            endif()
        elseif(WIN32) # TODO: this is temporary until an installer for windows is in place
            get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)
            set(PLUGIN_PATH_VST3 "${PLUGIN_PATH}/VST3")
        elseif(UNIX) # TODO: this is temporary until an installer for liunux is in place
            get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)
            set(PLUGIN_PATH_VST3 "${PLUGIN_PATH}/VST3")
        endif()
    endif()

    set(VST3VALIDATOR_BINARY_PATH $<TARGET_FILE:validator>)

    add_test(NAME ${target}_VST3_TEST
        COMMAND ${pluginval_SOURCE_DIR}/${PLUGINVAL_BINARY_PATH} --strictness-level 10 --skip-gui-tests --validate-in-process --vst3validator ${VST3VALIDATOR_BINARY_PATH} ${PLUGIN_PATH_VST3}/${PLUGIN_NAME}.vst3)

    if(APPLE AND NOT JUCE_BUILD_EXTRAS)
        add_test(NAME ${target}_AU_TEST
            COMMAND ${pluginval_SOURCE_DIR}/${PLUGINVAL_BINARY_PATH} --strictness-level 10 --skip-gui-tests --validate-in-process ${PLUGIN_PATH_AU}/${PLUGIN_NAME}.component)
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

# fetching and configure validator
FetchContent_Declare(
    vst3sdk
    GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk.git
    GIT_TAG v3.7.14_build_55
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    GIT_SUBMODULES base cmake pluginterfaces public.sdk
    GIT_SHALLOW TRUE
    GIT_SUBMODULES_RECURSE TRUE
    GIT_SUBMODULES_SHALLOW TRUE
    EXCLUDE_FROM_ALL
)
set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VSTGUI_SUPPORT OFF CACHE BOOL "" FORCE)
set(SMTG_RUN_VST_VALIDATOR OFF)
FetchContent_MakeAvailable(vst3sdk)

# if(TARGET validator)
# set_property(TARGET validator PROPERTY CXX_STANDARD 17)
# endif()