function(enable_pluginval_testing target)
    add_dependencies(${target} pluginval)

    get_target_property(PLUGIN_PATH ${target} LIBRARY_OUTPUT_DIRECTORY)

    add_custom_command(TARGET ${target}_VST3 POST_BUILD
        COMMAND 
            $<TARGET_FILE:pluginval>
            --skip-gui-tests --validate-in-process ${PLUGIN_PATH}/VST3/${target}.vst3
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )

    if(APPLE)
        add_custom_command(TARGET ${target}_AU POST_BUILD
        COMMAND 
            $<TARGET_FILE:pluginval>
            --skip-gui-tests --validate-in-process ${PLUGIN_PATH}/AU/${target}.component
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
    endif(APPLE)

endfunction(enable_pluginval_testing)
