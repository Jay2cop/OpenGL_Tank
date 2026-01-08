# OpenGL Tank Maze

OpenGL Tank Maze is a GLUT + GLEW playground where I built a drivable tank, coin-collecting maze, and skybox entirely from OBJ meshes to learn the shader-centric pipeline. Implementing my own camera manipulator, projectile physics, and text rendering forced me to internalize how matrix math, input handling, and OpenGL state all mesh together.

![Gameplay screenshot placeholder](assets/screenshot-placeholder.png)
*Drop final screenshots/GIFs inside `assets/` and update the link above once they exist.*

## Requirements
- **OS / GPU**: Linux (tested on Ubuntu) or Windows 10/11 with an OpenGL 3.3-compatible GPU and current drivers
- **Tooling**: Modern C++ compiler (g++/clang++ or MSVC 2019+), GNU Make (or mingw32-make), and optionally Qt's `qmake` if you want to regenerate the Makefile
- **Libraries**: OpenGL, GLU, GLEW, freeglut/GLUT, and the bundled `stb_image.h` header for texture loading
- **Assets**: OBJ meshes under `models/` and textures referenced within `Comp_vision_Game/TextureMap`; add marketing captures to the new `assets/` folder

## Build
### Linux
1. Install deps (example for Ubuntu): `sudo apt install build-essential freeglut3-dev glew-utils libglew-dev`
2. `cd Comp_vision_Game`
3. `make -j$(nproc)`
4. The binary `TextureMap` is emitted alongside the Makefile.

### Windows (MSYS2/MinGW or Visual Studio Developer Command Prompt)
1. Install a C++ toolchain plus OpenGL helpers (MSYS2: `pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut mingw-w64-x86_64-glew`).
2. `cd Comp_vision_Game`
3. Build with `mingw32-make -f Makefile` (or regenerate with `qmake qmake.pro` then run the chosen generator).
4. The output `TextureMap.exe` ends up in the same directory.

## Run
- Linux: `cd Comp_vision_Game && ./TextureMap`
- Windows: `cd Comp_vision_Game && TextureMap.exe`

Keep the working directory set to `Comp_vision_Game/` so relative paths like `../models/*.obj` and the texture folder resolve correctly.

## Controls
- `W` / `S`: drive the tank forward/backward
- `A` / `D`: skid steer the treads to rotate the hull
- Mouse move: rotate the turret
- Left mouse button: fire projectiles
- `Space`: toggle between front and rear chase cameras
- `Esc`: pause the game and show the menu
- Arrow Up/Down: move through menu items when paused
- `Enter`/`Return`: confirm the highlighted menu action

## Project structure
- `Comp_vision_Game/`: Gameplay entry point (`main.cpp`), GLSL shaders, textures, and the Makefile produced by `qmake`.
- `common/`: Math, shader, mesh, and camera helper classes shared by the project.
- `models/`: OBJ meshes for the tank body, wheels, maze tiles, coins, and skybox used at runtime.
- `assets/`: Reserved for README-ready screenshots and GIF captures (currently empty).

## Assets placeholder
Add stills or animated GIFs captured from the game into `assets/` (e.g., `assets/maze-run.gif`), then update the Markdown image links above so the README stays portfolio-ready.
