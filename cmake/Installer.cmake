function(create_installer target)
    get_target_property(PLUGIN_IS_COPIED ${target} JUCE_COPY_PLUGIN_AFTER_BUILD)

    if(PLUGIN_IS_COPIED)
        message(FATAL_ERROR "Plugin ${target} cant have COPY_PLUGIN_AFTER_BUILD set to TRUE while also call create_installer CMake function")
    endif()

    get_target_property(PLUGIN_NAME ${PROJECT_NAME} JUCE_PRODUCT_NAME)
    get_target_property(BUNDLE_ID ${target} JUCE_BUNDLE_ID)

    set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CPackConfig-${target}.cmake")
    set(CPACK_PACKAGE_NAME "${PLUGIN_NAME}")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PRODUCTBUILD_IDENTIFIER "${BUNDLE_ID}")
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
    set(CPACK_SYSTEM_NAME "")
    set(CPACK_PACKAGE_FILE_NAME "${target}-${CPACK_PACKAGE_VERSION}")

    if(APPLE)
        set(APP_INSTALL_DIRECTORY "Applications")
        set(VST3_INSTALL_DIRECTORY "Library/Audio/Plug-Ins/VST3")
    elseif(WIN32)
        set(APP_INSTALL_DIRECTORY ".")
        set(VST3_INSTALL_DIRECTORY ".")
    elseif(UNIX)
        set(APP_INSTALL_DIRECTORY "tmp")
        set(VST3_INSTALL_DIRECTORY "tmp")
    endif()

    get_target_property(product ${target}_VST3 RUNTIME_OUTPUT_DIRECTORY)
    install(DIRECTORY ${product}
        DESTINATION ${VST3_INSTALL_DIRECTORY}
        COMPONENT ${target}VST3
    )
    set(CPACK_COMPONENT_${target}VST3_DISPLAY_NAME "${PLUGIN_NAME} VST3")

    install(TARGETS ${target}_Standalone
        DESTINATION ${APP_INSTALL_DIRECTORY}
        COMPONENT ${target}APP
    )
    set(CPACK_COMPONENT_${target}APP_DISPLAY_NAME "${PLUGIN_NAME} Standalone")

    set(CPACK_COMPONENTS_ALL ${target}APP ${target}VST3)
    set(CPACK_COMPONENT_NAME "${target}")

    # Mac installer
    if(APPLE)
        include(CPackComponent)

        configure_file("${CMAKE_SOURCE_DIR}/cmake/component.plist.in" "component.plist" @ONLY)
        cpack_add_component(${target}APP PLIST "${CMAKE_CURRENT_BINARY_DIR}/component.plist")
        install(TARGETS ${target}_AU
            DESTINATION "Library/Audio/Plug-Ins/Components"
            COMPONENT ${target}AU
        )
        list(APPEND CPACK_COMPONENTS_ALL ${target}AU)

        set(CPACK_COMPONENT_${target}AU_DISPLAY_NAME "${PLUGIN_NAME} AU")

        set(CPACK_PACKAGING_INSTALL_PREFIX "/")
        set(CPACK_GENERATOR "productbuild")

    # Windows installer
    else()
        set(CPACK_GENERATOR "INNOSETUP")
        set(CPACK_INNOSETUP_USE_MODERN_WIZARD ON)

        include(CPackComponent)

        set(CPACK_INNOSETUP_${target}VST3_INSTALL_DIRECTORY "{commoncf64}")

        cpack_add_install_type(Full DISPLAY_NAME "Full installation")

        cpack_add_component(${target}APP
            DISPLAY_NAME "${PLUGIN_NAME} Standalone"
            INSTALL_TYPES Full
        )
        cpack_add_component(${target}VST3
            DISPLAY_NAME "${PLUGIN_NAME} VST3"
            INSTALL_TYPES Full
        )
    endif()

    # Linux installer
    include(CPack)
endfunction()