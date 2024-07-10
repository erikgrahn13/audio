find_program(DOCKER_EXECUTABLE docker)

if(NOT DOCKER_EXECUTABLE)
    message(WARNING "Docker not found. Please install Docker and try again.")
else()
    message(STATUS "Docker found: ${DOCKER_EXECUTABLE}")
endif()

# execute_process(
#     COMMAND docker build -t audio-doxygen docs/
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# )

# add_custom_command(
#     OUTPUT ${CMAKE_BINARY_DIR}/docs
#     COMMAND docker build -t audio-doxygen docs/
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     COMMENT "Building Docker container to generate documentation"
# )

add_custom_target(docs
    COMMAND docker build -t audio-doxygen docs/ && docker run --rm -v ${CMAKE_SOURCE_DIR}:/doxygen audio-doxygen
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running Docker container to generate documentation"
)
# if(WIN32)
#     set(DOXYGEN_URL "https://github.com/doxygen/doxygen/releases/download/Release_1_11_0/doxygen-1.11.0.windows.x64.bin.zip")
#     set(DOXYGEN_BINARY_PATH "doxygen.exe")
#     set(SHALL_NOT_EXTRACT FALSE)
# elseif(APPLE)
#     set(DOXYGEN_URL "https://github.com/doxygen/doxygen/releases/download/Release_1_11_0/Doxygen-1.11.0.dmg")
#     set(DOXYGEN_BINARY_PATH "Contents/Resources/doxygen")
#     set(SHALL_NOT_EXTRACT TRUE)
# elseif(UNIX)
#     set(DOXYGEN_URL "https://github.com/doxygen/doxygen/releases/download/Release_1_11_0/doxygen-1.11.0.linux.bin.tar.gz")
#     set(DOXYGEN_BINARY_PATH "bin/doxygen")
#     set(SHALL_NOT_EXTRACT FALSE)
# endif()

# FetchContent_Declare(
#   doxygen
#   URL ${DOXYGEN_URL}
#   DOWNLOAD_NO_EXTRACT ${SHALL_NOT_EXTRACT}
# )

# FetchContent_MakeAvailable(doxygen)
# if(APPLE)
#   execute_process(
#     COMMAND hdiutil attach ${doxygen_SOURCE_DIR}/Doxygen-1.11.0.dmg
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#   )

#   execute_process(
#     COMMAND ${CMAKE_COMMAND} -E copy_directory "/Volumes/Doxygen/Doxygen.app" ${doxygen_SOURCE_DIR}
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#   )

#   execute_process(
#     COMMAND hdiutil detach "/Volumes/Doxygen"
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#   )
# endif()


# Temporary to get a newer version of Doxygen on Ubuntu that supports all the new features,
# apt installs version 1.9.1 today and that version is too old for some features

# set(DOXYGEN_EXECUTABLE ${doxygen_SOURCE_DIR}/${DOXYGEN_BINARY_PATH})
# find_package(Doxygen)

# if(DOXYGEN_FOUND)
#     FetchContent_Declare(
#         doxygen-awesome
#         GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
#         GIT_TAG v2.3.3
#     )
#     FetchContent_MakeAvailable(doxygen-awesome)
    # Custom target to build docs using Docker
# add_custom_target(test-docker
#     COMMAND docker run --rm -v ${CMAKE_SOURCE_DIR}:/project -w /project my-dockerhub-image 
#         bash -c "cmake -B build/docs -S . && cmake --build build/docs --target docs"
#     COMMENT "Building documentation inside Docker container"
#     VERBATIM
# )

# add_custom_target(test-docker
#     COMMAND docker run --rm -it --entrypoint /bin/bash -v ${CMAKE_SOURCE_DIR}:/doxygen ghcr.io/doxygen/doxygen:release_1_11_0
#     COMMENT "Testing Docker container for Doxygen"
#     VERBATIM)


# add_custom_target(test-docker
#     COMMAND docker build ${CMAKE_SOURCE_DIR}/docs/ && docker run -it 
#     COMMENT "Testing Docker container for Doxygen"
#     VERBATIM)


    # set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)
    # set(DOXYGEN_EXCLUDE_PATTERNS */external/* *build/*)
    # set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome.css
    #                                 ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-sidebar-only.css
    #                                 ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-sidebar-only-darkmode-toggle.css
    #                                 ${CMAKE_SOURCE_DIR}/docs/custom.css)

    # set(DOXYGEN_HTML_HEADER ${CMAKE_SOURCE_DIR}/docs/header.html)
    # set(DOXYGEN_HTML_EXTRA_FILES ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    #                             ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
    #                             ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js)

    # set(DOXYGEN_GENERATE_TREEVIEW YES)
    # set(DOXYGEN_HTML_COLORSTYLE LIGHT)
    # set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build/docs)

    # doxygen_add_docs(
    #     docs
    #     ../
    #     COMMENT "Generate documentation"
    # )
# else()
#     message(STATUS "No doxygen found, please install it if you want to be able to generate documentation")
# endif()
