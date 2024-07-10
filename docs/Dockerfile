FROM ghcr.io/doxygen/doxygen:release_1_11_0

# Set environment variables to configure tzdata non-interactively
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC

# Install CMake
RUN apt-get update && apt-get install -y cmake git g++

# Create and set the working directory
WORKDIR /doxygen

# Copy your project files into the container
# COPY ../ /doxygen
ENTRYPOINT ["/bin/sh", "-c", "cmake -S /doxygen/docs -B /doxygen/build/docs && cmake --build /doxygen/build/docs --target docs"]