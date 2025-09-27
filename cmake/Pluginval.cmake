function(enable_pluginval_testing target)
    add_dependencies(${target} validator)
    get_target_property(bundle_id ${target} JUCE_BUNDLE_ID)

    if(NOT "${bundle_id}" MATCHES "${ABYSS_LOUNGE_BUNDLE_ID}.${target}")
        message(FATAL_ERROR "❗️ Plugin '${target}': Invalid bundle ID format. \nPlease add BUNDLE_ID \${ABYSS_LOUNGE_BUNDLE_ID}.\${PROJECT_NAME} to jude_add_plugin call for ${target}")
    endif()

    message(STATUS "✅ ${target}: ${bundle_id}")

    get_target_property(MY_SOURCES ${target} SOURCES)
    set(_werror_opts
      "$<$<CXX_COMPILER_ID:MSVC>:/WX>;$<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<COMPILE_LANGUAGE:CXX>>:-Werror>"
    )

  set_source_files_properties(${MY_SOURCES} PROPERTIES
    COMPILE_OPTIONS "${_werror_opts}"
  )

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
        elseif(WIN32)
            if(CMAKE_BUILD_TYPE STREQUAL "Debug")
                get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)
                set(PLUGIN_PATH_VST3 "${PLUGIN_PATH}/VST3")
            endif()
        elseif(UNIX) # TODO: this is temporary until an installer for liunux is in place
            if(CMAKE_BUILD_TYPE STREQUAL "Release")
                set(PLUGIN_PATH_VST3 "/usr/lib/vst3")
            else()
                get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)
                set(PLUGIN_PATH_VST3 "${PLUGIN_PATH}/VST3")
            endif()
        endif()
    endif()

    set(ENABLE_VST3VALIDATOR --vst3validator $<TARGET_FILE:validator>)
    # TODO: Temporary solution until vst3validator works on Linux with pluginval
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(ENABLE_VST3VALIDATOR "")
    endif()

    add_test(NAME ${target}_VST3_TEST
        COMMAND ${pluginval_SOURCE_DIR}/${PLUGINVAL_BINARY_PATH} --strictness-level 10 --skip-gui-tests --validate-in-process ${ENABLE_VST3VALIDATOR} ${PLUGIN_PATH_VST3}/${PLUGIN_NAME}.vst3)

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
set(CMAKE_CXX_STANDARD 17)
set(SMTG_CXX_STANDARD "17" CACHE STRING "" FORCE)
add_compile_definitions(_SILENCE_CXX20_U8PATH_DEPRECATION_WARNING)
FetchContent_MakeAvailable(vst3sdk)
set(CMAKE_CXX_STANDARD 23)

# if(TARGET sdk_hosting)
# target_compile_features(sdk_hosting PUBLIC cxx_std_17)
# endif()

# if(TARGET validator)
# target_compile_features(validator PUBLIC cxx_std_23)
# endif()
