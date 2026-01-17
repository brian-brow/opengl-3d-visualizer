A basic opengl based 3D visualizer. A work in progress for now but I plan to develop into a functional 3D engine.
Currently just displays one large Utah Teapot surrounded by smaller teapots.

Controls:
WASD to move the camera horizontally
Left and Right arrow keys to rotate the teapots
Spacebar to make the big one jump

Setup:

Arch Linux:
sudo pacman -S --needed cmake sdl2 sdl2_image sdl2_mixer glew glm base-devel && mkdir -p build && cd build && cmake .. && make && ./bin/visualizer

Windows (Powershell, requires vcpkg already installed):
vcpkg install sdl2 sdl2-image sdl2-mixer glew glm; if ($?) { mkdir build -Force; cd build; cmake .. -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"; cmake --build .; if ($?) { .\bin\Debug\visualizer.exe } }

(this was developed on linux I have not tested the windows command yet)
