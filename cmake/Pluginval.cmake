function(enable_pluginval_testing target)
    get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)

    add_test(NAME ${target}_VST3_TEST 
             COMMAND ${pluginval_SOURCE_DIR}/${PLUGINVAL_BINARY_PATH} --strictness-level 10 --verbose --skip-gui-tests --validate-in-process ${PLUGIN_PATH}/VST3/${target}.vst3)

    if(APPLE AND NOT JUCE_BUILD_EXTRAS)
        # Make the post_build_command target depend on all your other targets
        add_dependencies(post_build_command ${target})
        add_test(NAME ${target}_AU_TEST 
        COMMAND ${pluginval_SOURCE_DIR}/${PLUGINVAL_BINARY_PATH} --strictness-level 10 --verbose --skip-gui-tests --validate-in-process $ENV{HOME}/Library/Audio/Plug-Ins/Components/${target}.component)
    endif(APPLE AND NOT JUCE_BUILD_EXTRAS)

endfunction(enable_pluginval_testing)

# Create a custom target for restarting the AudioComponentRegistrar to be able to register AU plugins to the system
if(APPLE AND NOT JUCE_BUILD_EXTRAS)
    add_custom_target(
        post_build_command ALL
        COMMAND pgrep -x AudioComponentRegistrar >/dev/null && killall -9 AudioComponentRegistrar; ${CMAKE_COMMAND} -E echo "killed AudioComponentRegistrar" || ${CMAKE_COMMAND} -E echo "AudioComponentRegistrar Process not found"
    )
endif()

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
)
FetchContent_MakeAvailable(pluginval)
