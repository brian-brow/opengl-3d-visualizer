# Assignment 3 – OpenGL Model Transform Viewer

This program loads triangular meshes from `.obj` files and renders them using modern OpenGL.  
Two models (Utah Teapot and Blender Monkey) are displayed and can be animated around a shared axis.

--------------------------------------------------

CONTROLS

W / A / S / D
Move the camera in the scene

Mouse Movement
Rotate the camera view

Left / Right Arrow Keys
Rotate the objects

R
Toggle animation that rotates the two objects around the axis between their centers

--------------------------------------------------

BUILD INSTRUCTIONS

Dependencies (Arch Linux)

Install required libraries:

sudo pacman -S --needed base-devel sdl2 sdl2_image sdl2_mixer glew glm

--------------------------------------------------

COMPILE

Run the following command in the project root directory:

make

This compiles the program and creates the executable:

bin/gl3d

--------------------------------------------------

RUN THE PROGRAM

After compiling, run:

make run

or directly:

./bin/gl3d

--------------------------------------------------

PROJECT STRUCTURE

include/
    Camera.h
    Mesh.h
    Shader.h
    Texture.h
    FileLoader.h
    
src/
    main.cpp
    Camera.cpp
    Mesh.cpp
    Shader.cpp
    Texture.cpp
    FileLoader.cpp

assets/
    audio/
    models/
    shaders/
    textures/

bin/
    (compiled executable)

--------------------------------------------------

NOTES

- Mesh geometry is loaded from `.obj` files.
- Rendering uses vertex and fragment shaders loaded from external files.
- Each model has its own VAO, model matrix, and transformation pipeline.
- Tested on Arch Linux with OpenGL 3.3 and SDL2.
