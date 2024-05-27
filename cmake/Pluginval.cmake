function(enable_pluginval_testing target)
    add_dependencies(${target} pluginval)
    add_custom_command(TARGET ${target}_VST3 POST_BUILD
        COMMAND 
            $<TARGET_FILE:pluginval>
            --skip-gui-tests --validate-in-process $<TARGET_FILE:${target}_VST3>
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endfunction(enable_pluginval_testing)
