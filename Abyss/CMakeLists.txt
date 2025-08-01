# Example Audio Plugin CMakeLists.txt

# To get started on a new plugin, copy this entire folder (containing this file and C++ sources) to
# a convenient location, and then start making modifications.

# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.

cmake_minimum_required(VERSION 3.22)

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.
project(Abyss VERSION 0.0.1)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
# add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# (AudioPluginExample here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.
juce_add_plugin(${PROJECT_NAME}

    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # COMPANY_NAME ...                          # Specify the name of the plugin's author
    BUNDLE_ID ${ABYSS_LOUNGE_BUNDLE_ID}.${PROJECT_NAME}

    # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
    # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE ${ABYSS_LOUNGE_PLUGIN_MANUFACTURER_CODE} # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE AbyS # A unique four-character plugin id with exactly one upper-case character
    NEEDS_WEBVIEW2 TRUE # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 Standalone # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME ${PROJECT_NAME}) # The name of the final executable, which can differ from the target name

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.
juce_generate_juce_header(${PROJECT_NAME})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.1)
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS})
endif()

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.
target_sources(${PROJECT_NAME}
    PRIVATE
    AbyssPluginEditor.cpp
    AbyssPluginProcessor.cpp)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.
target_compile_definitions(${PROJECT_NAME}
    PUBLIC

    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0 # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# The things below should probably be moved to a cmake script in a cmake function
# file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/ui")

# if(CMAKE_BUILD_TYPE STREQUAL "Release")
# file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/ui/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/ui/")
# file(COPY "${juce_SOURCE_DIR}/modules/juce_gui_extra/native/javascript/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/ui/juce/")
# file(COPY "${CMAKE_SOURCE_DIR}/widgets/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/ui/widgets/")

# set(WEBVIEW_FILES_ZIP_NAME "webview_files.zip")
# set(TARGET_WEBVIEW_FILES_ZIP_PATH "${CMAKE_CURRENT_BINARY_DIR}/${WEBVIEW_FILES_ZIP_NAME}")

# add_custom_target(ZipWebViewFiles
# COMMAND
# ${CMAKE_COMMAND} -E tar cvf
# "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
# --format=zip
# "${CMAKE_CURRENT_BINARY_DIR}/ui"
# BYPRODUCTS
# "${TARGET_WEBVIEW_FILES_ZIP_PATH}"
# WORKING_DIRECTORY
# "${CMAKE_CURRENT_BINARY_DIR}"
# COMMENT "Zipping WebView files..."
# VERBATIM
# )

# target_compile_definitions(${PROJECT_NAME} PRIVATE ZIPPED_FILES_PREFIX="ui/")

# juce_add_binary_data(WebViewFiles
# HEADER_NAME WebViewFiles.h
# NAMESPACE webview_files
# SOURCES ${TARGET_WEBVIEW_FILES_ZIP_PATH}
# )

# add_dependencies(WebViewFiles ZipWebViewFiles)
# target_link_libraries(${PROJECT_NAME} PRIVATE WebViewFiles)

# elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
# set(LINK_DEST "${CMAKE_CURRENT_BINARY_DIR}/ui")
# set(SRC_UI "${CMAKE_CURRENT_SOURCE_DIR}/ui")
# set(SRC_WIDGETS "${CMAKE_SOURCE_DIR}/widgets")
# set(SRC_JUCE_JS "${juce_SOURCE_DIR}/modules/juce_gui_extra/native/javascript")

# add_custom_command(
# OUTPUT "${LINK_DEST}"
# COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_UI}/index.html" "${LINK_DEST}/index.html"
# COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_UI}/style.css" "${LINK_DEST}/style.css"
# COMMENT "Symlinking UI directory"
# VERBATIM
# )

# add_custom_command(
# OUTPUT "${LINK_DEST}/widgets"
# COMMAND ${CMAKE_COMMAND} -E make_directory "${LINK_DEST}"
# COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_WIDGETS}" "${LINK_DEST}/widgets"
# COMMENT "Symlinking widgets directory"
# VERBATIM
# )

# add_custom_command(
# OUTPUT "${LINK_DEST}/juce"
# COMMAND ${CMAKE_COMMAND} -E make_directory "${LINK_DEST}"
# COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_JUCE_JS}" "${LINK_DEST}/juce"
# COMMENT "Symlinking JUCE JavaScript directory"
# VERBATIM
# )

# add_custom_target(CreateWebviewSymlinks
# DEPENDS "${LINK_DEST}" "${LINK_DEST}/widgets" "${LINK_DEST}/juce"
# )

# add_dependencies(${PROJECT_NAME} CreateWebviewSymlinks)
# endif()
enable_webview(
    ui/index.html
    ui/style.css
)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_audio_utils` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.
target_link_libraries(${PROJECT_NAME}
    PRIVATE

    # AudioPluginData           # If we'd created a binary data target, we'd link to it here
    juce::juce_audio_utils
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_include_directories(${PROJECT_NAME} PRIVATE ${juce_SOURCE_DIR}/examples/Assets)

if(JUCE_BUILD_EXTRAS)
    add_dependencies(${PROJECT_NAME} AudioPluginHost AudioFilePlayer_VST3)
endif()

enable_pluginval_testing(${PROJECT_NAME})