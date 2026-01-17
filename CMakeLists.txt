cmake_minimum_required(VERSION 3.10)
project(opengl_3d_visualizer VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output binaries to build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/FileLoader.cpp
)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Platform-specific package finding for SDL2_image and SDL2_mixer
if(WIN32)
    # On Windows, you might need to set SDL2_IMAGE_DIR and SDL2_MIXER_DIR
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
else()
    # On Linux/Mac, use pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
endif()

# Include directories for all dependencies
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
)

# Create executable
add_executable(visualizer ${SOURCES})

# Link libraries
target_link_libraries(visualizer
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
)

# Platform-specific linking
if(WIN32)
    target_link_libraries(visualizer
        mingw32
        SDL2main
    )
endif()

# Copy assets directory to build directory
add_custom_command(TARGET visualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "OpenGL found: ${OPENGL_FOUND}")
message(STATUS "SDL2 found: ${SDL2_FOUND}")
