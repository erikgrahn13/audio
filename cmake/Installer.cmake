function(create_installer target)
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
    elseif(WIN32) # fix correct destinations when inno setup support is added
        set(APP_INSTALL_DIRECTORY "${CMAKE_BINARY_DIR}/tmp")
        set(VST3_INSTALL_DIRECTORY "${CMAKE_BINARY_DIR}/tmp")
    endif()

    install(TARGETS ${target}_VST3
        DESTINATION ${VST3_INSTALL_DIRECTORY}
        COMPONENT ${target}VST3
    )

    install(TARGETS ${target}_Standalone
        DESTINATION ${APP_INSTALL_DIRECTORY}
        COMPONENT ${target}APP
    )

    set(CPACK_COMPONENTS_ALL ${target}APP ${target}VST3)
    set(CPACK_COMPONENT_NAME "${target}")

    # Mac installer
    if(APPLE)
        include(CPackComponent)
        set(_entries "")
        string(APPEND _entries
            "  <dict>
            <key>RootRelativeBundlePath</key><string>Applications/${PLUGIN_NAME}.app</string>
            <key>BundleIsRelocatable</key><false/>
            <key>BundleHasStrictIdentifier</key><true/>
            <key>BundleIsVersionChecked</key><true/>
        </dict>
        ")

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
    elseif(WIN32)
        set(CPACK_GENERATOR "TGZ")
    endif()

    # Linux installer
    include(CPack)
endfunction()