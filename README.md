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

This section provides a high-level description of how to manually build the Tactile editor. The
project makes use of [Vcpkg](https://github.com/microsft/vcpkg) for third-party dependency
management. As a result, building the code base is really quite straightforward. The process is also
basically the same on all platforms.

You can install Vcpkg wherever you like, so it helps to set the environment variable `VCPKG_ROOT` to
the path where you've installed Vcpkg.

### Windows

```cmd
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

```cmd
copy "cmake\x64-tactile.cmake" "%VCPKG_ROOT%\triplets\community\x64-tactile.cmake"
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-tactile -GNinja
```

### Linux (Ubuntu)

```bash
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh
```

```bash
mv "cmake/x64-tactile-linux.cmake" "$VCPKG_ROOT/triplets/community/x64-tactile-linux.cmake"
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-tactile-linux -GNinja
ninja
```

### macOS

```bash
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh
```

```bash
mv "cmake/x64-tactile-osx.cmake" "$VCPKG_ROOT/triplets/community/x64-tactile-osx.cmake"
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-tactile-osx -GNinja
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
