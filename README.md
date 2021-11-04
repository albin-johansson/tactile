# Tactile

[![License](https://img.shields.io/badge/license-GPL3-blue.svg)](https://opensource.org/licenses/GPL-3.0)
[![version](https://img.shields.io/github/v/release/albin-johansson/tactile)](https://github.com/albin-johansson/tactile/releases)
[![CI: Windows](https://github.com/albin-johansson/tactile/actions/workflows/windows.yml/badge.svg?branch=dev)](https://github.com/albin-johansson/tactile/actions/workflows/windows.yml)
[![CI: Ubuntu](https://github.com/albin-johansson/tactile/actions/workflows/ubuntu.yml/badge.svg?branch=dev)](https://github.com/albin-johansson/tactile/actions/workflows/ubuntu.yml)
[![CI: macOS](https://github.com/albin-johansson/tactile/actions/workflows/macos.yml/badge.svg?branch=dev)](https://github.com/albin-johansson/tactile/actions/workflows/macos.yml)

A tilemap editor that aims to be easy-to-use, fast and lightweight.

![example](meta/splash.png "splash")

## Aim

* Must be easy to learn
* Must be easy to use
* Keep things simple, hide advanced options that are rarely used
* Well documented and high-quality source code
* Be lightweight and performant

## Features

* Great performance thanks to ECS architecture
* Extensive and intelligent undo/redo support
* Tile layers
* Object layers
* Group layers
* Embedded and external tilesets
* Supports an intuitive and human-readable YAML map format
* Read and write support for the JSON and XML map formats used by [Tiled](https://www.mapeditor.org/)
* Intuitive mouse tools
  * Tile stamp
  * Eraser
  * Bucket fill
* Display tile animations
* Various editor themes
* Insightful and helpful error messages when things go wrong, e.g. when parsing corrupted maps

## Coming features

* Object layer tools
* Active layer highlighting
* Stamp tool randomizer
* Tile animation editor
* Minimap dock
* Components
* Tileset editor

## Building

This section provides a high-level description of how to manually build the Tactile editor. A few of
the third-party libraries used in the code base must be separately installed on your system, but
most of the dependencies are either bundled in the repository or downloaded at build-time through
CMake.

### Windows

Firstly, you will need to download the headers and development binaries SDL2, SDL2_ttf and
SDL2_image. Subsequently, the directories where you have put the SDL libraries should be accessible
through environment variables called `SDL2DIR`, `SDL2TTFDIR` and `SDL2IMAGEDIR`, respectively. For
example, running `echo %SDL2DIR%` should result in something like `C:\dev\SDL2-2.0.16` being output.
Then, put the runtime binaries (`.dll` files), in a folder called `bin` in the root directory of the
repository. After you have done this, you will need to repeat this process for GLEW, where you
define a `GLEWDIR` environment variable and copy the GLEW `.dll` to the `bin` directory. In addition
to these dependencies, you'll need CMake and MSVC installed, the latter is achieved by simply
installing Visual Studio. Ninja isn't actually required, but is highly recommended.

```cmd
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -GNinja
ninja
```

### Linux (Ubuntu)

Building the source code on Linux, in this case Ubuntu, should be relatively straightforward using
your package manager. At the time of writing, the packaged versions of SDL2 and Protobuf are too
old, so you'll have to build them by yourself. The following assumes that you've got a C++ compiler
installed such as GCC.

```bash
sudo apt install cmake ninja-build libsdl2-ttf-dev libsdl2-image-dev libglew-dev

curl -L https://www.libsdl.org/release/SDL2-2.0.16.tar.gz | tar xz
cd SDL2-2.0.16
./configure
make -j 8
sudo make install
cd ..

curl -L https://github.com/protocolbuffers/protobuf/releases/download/v3.18.1/protobuf-cpp-3.18.1.tar.gz | tar xz
cd protobuf-3.18.1
./configure
make -j 8
sudo make install
sudo ldconfig
cd ..

git clone https://github.com/albin-johansson/tactile
cd tactile
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -GNinja
ninja
```

### macOS

The easiest platform to get everything configured correctly with is macOS using Brew. The only 
package that doesn't match our needs is Protobuf.

```bash
brew update
brew install llvm
brew install cmake
brew install ninja
brew install mpg123
brew install glfw
brew install glew
brew install SDL2
brew install SDL2_ttf
brew install SDL2_image

brew install autoconf
brew install automake
brew install libtool
curl -L https://github.com/protocolbuffers/protobuf/releases/download/v3.18.1/protobuf-cpp-3.18.1.tar.gz | tar xz
cd protobuf-3.18.1
./configure
make -j 8
sudo make install

git clone https://github.com/albin-johansson/tactile
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -GNinja
ninja
```

## Dependencies

* [Centurion](https://github.com/albin-johansson/centurion)
* [Dear ImGui](https://github.com/ocornut/imgui)
* [EnTT](https://github.com/skypjack/entt)
* [fmt](https://github.com/fmtlib/fmt)
* [GLEW](https://github.com/nigels-com/glew)
* [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders)
* [JSON for Modern C++](https://github.com/nlohmann/json)
* [Magic Enum C++](https://github.com/Neargye/magic_enum)
* [Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs)
* [Protocol Buffers](https://github.com/protocolbuffers/protobuf)
* [pugixml](https://github.com/zeux/pugixml)
* [SDL2](https://github.com/libsdl-org/SDL)
* [SDL2_image](https://github.com/libsdl-org/SDL_image)
* [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
* [stb_image](https://github.com/nothings/stb)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)
