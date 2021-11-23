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
project makes use of [Vcpkg](https://github.com/microsoft/vcpkg) for third-party dependency
management. As a result, building the code base is really quite straightforward. The process is also
basically the same on all platforms.

### Install Vcpkg

You can install Vcpkg wherever you like, so it helps to set the environment variable `VCPKG_ROOT` to
the path where you've installed Vcpkg. You can download Vcpkg through `git clone https://github.com/microsoft/vcpkg`. After you've downloaded Vcpkg, install it using the following command in the root of the downloaded repository.

|   OS    | Command                       |
| :-----: | :---------------------------- |
| Windows | `.\vcpkg\bootstrap-vcpkg.bat` |
|  Linux  | `./vcpkg/bootstrap-vcpkg.sh`  |
|  macOS  | `./vcpkg/bootstrap-vcpkg.sh`  |

### Install Tactile triplet files

Then we want to make the custom Tactile triplet files available for use by Vcpkg, the following shows how you can do this (you can of course copy the file by hand, but that's no fun). These commands assume that you are in the root of the Tactile repository.

|   OS    | Command                                                                                              |
| :-----: | :--------------------------------------------------------------------------------------------------- |
| Windows | `copy "cmake\x64-tactile-windows.cmake" "%VCPKG_ROOT%\triplets\community\x64-tactile-windows.cmake"` |
|  Linux  | `mv "cmake/x64-tactile-linux.cmake" "$VCPKG_ROOT/triplets/community/x64-tactile-linux.cmake"`        |
|  macOS  | `mv "cmake/x64-tactile-osx.cmake" "$VCPKG_ROOT/triplets/community/x64-tactile-osx.cmake"`            |

### Build the project

Now, we can generate the build files. You don't have to Ninja, but it is highly recommended. On all operating systems, you should create a separate build directory where all of our generated build files and (later) binaries will end up.

```bash
mkdir build
cd build
```

Use the following commands to tell CMake to generate the build files. Note, if you are using an IDE, such as JetBrains CLion, you can specify these CMake parameters in your CMake profile settings so that you can just build the project as per usual. In CLion 2021.2, you'll access these settings in `View -> Tool Windows -> CMake`, and then selecting `CMake Settings`. You can also just search for `CMake Settings` with `Shift+Shift`. Subsequently, in the `CMake Settings` window, add the parameters to the `CMake options` field.

|   OS    | Command                                                                                                                            |
| :-----: | :--------------------------------------------------------------------------------------------------------------------------------- |
| Windows | `cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-tactile-windows -GNinja` |
|  Linux  | `cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-tactile-linux -GNinja`    |
|  macOS  | `cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-tactile-osx -GNinja`      |

Finally, all we need to build the entire project is then to invoke Ninja (or whatever generator you use).

```bash
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
* [stb_image](https://github.com/nothings/stb)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)
