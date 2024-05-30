function(enable_pluginval_testing target)
    add_dependencies(${target} pluginval)

    get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)

    add_test(NAME ${target}_VST3_TEST 
             COMMAND $<TARGET_FILE:pluginval> --strictness-level 10 --verbose --skip-gui-tests --validate-in-process ${PLUGIN_PATH}/VST3/${target}.vst3)

    # add_custom_command(TARGET  ${target}_VST3
    #                    POST_BUILD
    #                    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R ${target}_TEST
    #                    COMMENT "Running MyTest as a post-build step")
    
    # add_custom_command(TARGET ${target}_VST3 POST_BUILD
    #     COMMAND 
    #         $<TARGET_FILE:pluginval>
    #         --skip-gui-tests --validate-in-process ${PLUGIN_PATH}/VST3/${target}.vst3
    #         WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    # )
    option(CI_BUILD "Enables pluginval tests of AU plugins" OFF)

    if(APPLE AND CI_BUILD)
        add_test(NAME ${target}_AU_TEST 
        COMMAND $<TARGET_FILE:pluginval> --strictness-level 10 --verbose --skip-gui-tests --validate-in-process $ENV{HOME}/Library/Audio/Plug-Ins/Components/${target}.component)

        add_custom_command(TARGET ${target}_AU POST_BUILD
            COMMAND mkdir -p $ENV{HOME}/Library/Audio/Plug-Ins/Components
            COMMAND ${CMAKE_COMMAND} -E remove_directory "$ENV{HOME}/Library/Audio/Plug-Ins/Components/${target}.component"
            COMMAND mv ${PLUGIN_PATH}/AU/${target}.component $ENV{HOME}/Library/Audio/Plug-Ins/Components/${target}.component
            # COMMAND ${CMAKE_COMMAND} -E rename "${PLUGIN_PATH}/AU/${target}.component" "$ENV{HOME}/Library/Audio/Plug-Ins/Components"
            COMMAND pgrep -x AudioComponentRegistrar >/dev/null && killall -9 AudioComponentRegistrar && echo "Killed AudioComponentRegistrar" || echo "AudioComponentRegistrar Process not found"
            # COMMAND 
            # $<TARGET_FILE:pluginval>
            # --skip-gui-tests --validate-in-process $ENV{HOME}/Library/Audio/Plug-Ins/Components/${target}.component
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
    endif(APPLE AND CI_BUILD)

endfunction(enable_pluginval_testing)
