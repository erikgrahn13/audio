function(create_installer target INSTALL_IMAGE)
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
    # Place installer only in CPack packages directory to avoid duplicates
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/installers")
    set(CPACK_SYSTEM_NAME "")
    set(CPACK_PACKAGE_FILE_NAME "${target}-${CPACK_PACKAGE_VERSION}")

    if(APPLE)
        set(APP_INSTALL_DIRECTORY "Applications")
        set(VST3_INSTALL_DIRECTORY "Library/Audio/Plug-Ins/VST3")
    elseif(WIN32)
        set(APP_INSTALL_DIRECTORY ".")
        set(VST3_INSTALL_DIRECTORY ".")
    elseif(UNIX)
        set(APP_INSTALL_DIRECTORY ".")
        set(VST3_INSTALL_DIRECTORY ".")
    endif()

    set(CPACK_COMPONENTS_ALL)
    include(CPackComponent)

    if(TARGET ${target}_VST3)
        get_target_property(VST3_PATH ${target}_VST3 JUCE_PLUGIN_ARTEFACT_FILE)
        install(DIRECTORY ${VST3_PATH}
            DESTINATION ${VST3_INSTALL_DIRECTORY}
            COMPONENT ${target}VST3
            PATTERN "Plugin.ico" EXCLUDE
        )
        list(APPEND CPACK_COMPONENTS_ALL ${target}VST3)
        cpack_add_component(${target}VST3
            DISPLAY_NAME "${PLUGIN_NAME} VST3"
            INSTALL_TYPES Full
        )
    endif()
    
    if(TARGET ${target}_Standalone)
        get_target_property(APP_PATH ${target}_Standalone JUCE_PLUGIN_ARTEFACT_FILE)
        install(PROGRAMS ${APP_PATH}
            DESTINATION ${APP_INSTALL_DIRECTORY}
            COMPONENT ${target}APP
        )
        list(APPEND CPACK_COMPONENTS_ALL ${target}APP)
        cpack_add_component(${target}APP
            DISPLAY_NAME "${PLUGIN_NAME} Standalone"
            INSTALL_TYPES Full
        )
    endif()

    set(CPACK_COMPONENT_NAME "${target}")

    # Mac installer
    if(APPLE)
        configure_file("${CMAKE_SOURCE_DIR}/cmake/component.plist.in" "component.plist" @ONLY)
        cpack_add_component(${target}APP 
            DISPLAY_NAME "${PLUGIN_NAME} Standalone"
            PLIST "${CMAKE_CURRENT_BINARY_DIR}/component.plist"
        )
        install(TARGETS ${target}_AU
            DESTINATION "Library/Audio/Plug-Ins/Components"
            COMPONENT ${target}AU
        )
        list(APPEND CPACK_COMPONENTS_ALL ${target}AU)

        cpack_add_component(${target}AU
            DISPLAY_NAME "${PLUGIN_NAME} AU"
            INSTALL_TYPES Full
        )

        set(CPACK_PACKAGING_INSTALL_PREFIX "/")
        set(CPACK_GENERATOR "productbuild")
        
    # Windows installer
    elseif(WIN32)
        set(CPACK_GENERATOR "INNOSETUP")
        set(CPACK_INNOSETUP_USE_MODERN_WIZARD ON)
        set(CPACK_INNOSETUP_IGNORE_LICENSE_PAGE ON)
        set(CPACK_INNOSETUP_SETUP_WizardStyle dark)
        set(CPACK_INNOSETUP_SETUP_WizardBackImageFile ${INSTALL_IMAGE})
        set(CPACK_INNOSETUP_SETUP_DisableWelcomePage NO)
        set(CPACK_INNOSETUP_SETUP_SetupIconFile "${CMAKE_SOURCE_DIR}/resources/assets/logo_transparent.ico")
        set(CPACK_INNOSETUP_${target}VST3_INSTALL_DIRECTORY "{commoncf64}/VST3")

        cpack_add_install_type(Full DISPLAY_NAME "Full installation")
    else()
        set(CPACK_GENERATOR "TGZ")
        configure_file("${CMAKE_SOURCE_DIR}/cmake/install.sh.in" "install.sh" @ONLY)
        
        # Add the install script to the tarball
        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/install.sh"
            DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                       GROUP_READ GROUP_EXECUTE
                       WORLD_READ WORLD_EXECUTE
            COMPONENT ${target}SCRIPT
        )
        
        # Include the script component in the package
        list(APPEND CPACK_COMPONENTS_ALL ${target}SCRIPT)
    endif()

    # Explicitly set which components to include (prevents other targets' components from being included)
    set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL})
    
    # Install only from this target's subdirectory to avoid installing other targets' components
    set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};${PROJECT_NAME};ALL;/")

    # Linux installer
    include(CPack)
endfunction()
