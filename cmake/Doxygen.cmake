find_program(DOCKER_EXECUTABLE docker)

if(NOT DOCKER_EXECUTABLE)
    message(WARNING "Docker not found. Please install Docker and try again.")
else()
    message(STATUS "Docker found: ${DOCKER_EXECUTABLE}")

    add_custom_target(docs
        COMMAND docker build -t audio-doxygen docs/ && docker run --rm -v ${CMAKE_SOURCE_DIR}:/doxygen audio-doxygen
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running Docker container to generate documentation"
    )
endif()
