# Overview

This is a demo project that is attempting to integrate the 3D graphics library Ogre Next v4.0 with a GUI overlay provided by RmlUi v6.1. This is a crucial element for making video games that have menu systems, debug screens, etc.

The project is written in C++ using CMake as the build system, and has been tested on Debian 12 (bookworm) Linux. Currently, some X11 specific code is being used to pass the window "handle" from SDL2 to Ogre, but besides that it should be relatively cross-platform because of using SDL2 and OpenGL.

# Dependencies

* [CMake](https://gitlab.kitware.com/cmake/cmake) (tested with v3.25.1)
* [GCC](https://gcc.gnu.org/) (tested with v12.2.0)
* [Ogre Next](https://github.com/OGRECave/ogre-next) v4.0 (v3.0 might also work?)
* [RmlUi](https://github.com/mikke89/RmlUi) (tested with v6.1)
* [SDL2](https://github.com/libsdl-org/SDL) (tested with v2.26.5)
* [Open Asset Import Library](https://github.com/assimp/assimp) aka "assimp" (tested with v5.2.5)

## Ogre Next

Here is how to install Ogre Next v4.0 (the master branch on git) locally, such that it is installed inside the user folder rather than system-wide.

NOTE: Ogre Next v3.0 might also work, but I was having trouble setting it up and decided to go with the master branch as a result.

Installing the Ogre Next dependencies required for compiling it:

```
sudo apt install libfreeimage-dev libxaw7-dev libxrandr-dev libgl1-mesa-dev
```

NOTE: the above dependencies will implicitly pull in these other dependencies that Ogre Next requires:

* `libx11-dev`
* `libxt-dev`

Then we must clone the repository:

```
git clone --recursive https://github.com/OGRECave/ogre-next.git
```

And finally within the cloned directory, we can build and install using `cmake`:

```
mkdir build
cmake .. -DCMAKE_INSTALL_PREFIX=~/apps/ogre-next
cmake --build . --config Release --target install
```

## RmlUi

Clone the git repository:

```
git clone --recursive https://github.com/mikke89/RmlUi.git
```

Build it:

```
cd RmlUi
mkdir build
cd build
cmake .. --preset samples -DRMLUI_BACKEND=SDL_GL3
```

## Open Asset Import Library

Installing under Debian 12 (bookworm):

```
sudo apt install libassimp-dev
```

## SDL2

Installing under Debian 12 (bookworm):

```
sudo apt install libsdl2-dev
```

# Compiling

First, you must edit some hard-coded paths in `CMakeLists.txt` to point to the installed Ogre Next and to the built (but not installed) RmlUi:

```cmake
list(APPEND CMAKE_PREFIX_PATH "/home/USERNAME/apps/ogre-next")
list(APPEND CMAKE_MODULE_PATH "/home/USERNAME/apps/ogre-next/lib/OGRE-Next/cmake/")
list(APPEND CMAKE_PREFIX_PATH "/home/USERNAME/repos/RmlUi/build")
list(APPEND CMAKE_MODULE_PATH "/home/USERNAME/repos/RmlUi/build/install/")

set(RmlUi_DIR /home/USERNAME/repos/ext/RmlUi/build)
if(RmlUi_STATIC)
  set(RmlUi_STATIC /home/USERNAME/repos/RmlUi/build/lib)
endif()
```

NOTE: you must "hard-code" the paths and not use the tilde (~) symbol, as it doesn't always evaluate properly.

Then you should be able to build this repository:

```
mkdir build
cd build
cmake ..
```

# Running

```
./OgreNextRmlUiDemo
```

* <kbd>TAB</kbd> to toggle between the RmlUi main menu and the game in mouselook mode
* <kbd>F8</kbd> to toggle RmlUi's debugger
* <kbd>ESC</kbd> to quit the program

NOTE: it is assumed that you are inside of a `build` type directory within the main project folder, therefore you can access assets using a relative path `../data` and `../assets`:

NOTE: it is assumed that Ogre Next was installed to `~/apps/ogre-next`, and this project builds that path using the Linux APIs to get the home directory location.
