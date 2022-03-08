# Tactile

[![License](https://img.shields.io/badge/license-GPL3-blue.svg)](https://opensource.org/licenses/GPL-3.0)
[![version](https://img.shields.io/github/v/release/albin-johansson/tactile)](https://github.com/albin-johansson/tactile/releases)
[![CI: Windows](https://github.com/albin-johansson/tactile/actions/workflows/windows.yml/badge.svg?branch=dev)](https://github.com/albin-johansson/tactile/actions/workflows/windows.yml)
[![CI: Ubuntu](https://github.com/albin-johansson/tactile/actions/workflows/ubuntu.yml/badge.svg?branch=dev)](https://github.com/albin-johansson/tactile/actions/workflows/ubuntu.yml)
[![CI: macOS](https://github.com/albin-johansson/tactile/actions/workflows/macos.yml/badge.svg?branch=dev)](https://github.com/albin-johansson/tactile/actions/workflows/macos.yml)

A tilemap editor that aims to be easy-to-use, fast and lightweight.

![example](meta/splash.png "splash")

## Aim

* Easy to learn *and* easy to use
* Workflow optimized for common actions
* Well documented and high-quality source code
* Lightweight feel and scalable performance

## Features

* Great performance thanks to ECS architecture
* Extensive and intuitive undo & redo support
* Various layer types
    * Tile layers
    * Object layers
    * Group layers
* Supports an intuitive and human-readable YAML map format
* Read and write support for the JSON and XML map formats used by [Tiled](https://www.mapeditor.org/)
    * Note that not all Tiled features are supported by Tactile
* Intuitive mouse tools
  * Tile stamp tool
  * Eraser tool
  * Bucket fill tool
  * Rectangle tool
  * Point tool
* Components (attachable bundles of attributes, otherwise commonly found in game engines)
* Tile animations
* Various editor themes
* Insightful and helpful error messages when things go wrong, e.g. when parsing corrupted maps

## Building

This section provides a high-level description of how to manually build the Tactile editor. The project makes use
of [Vcpkg](https://github.com/microsoft/vcpkg) for third-party dependency management. As a result, building the code
base is really quite straightforward. The process is also basically the same on all platforms.

### Install Vcpkg

You can install Vcpkg wherever you like, so it helps to set the environment variable `VCPKG_ROOT` to the path where
you've installed Vcpkg. You can download Vcpkg through `git clone https://github.com/microsoft/vcpkg`. After you've
downloaded Vcpkg, install it using the following command in the root of the downloaded repository.

|   OS    | Command                       |
|:-------:|:------------------------------|
| Windows | `.\vcpkg\bootstrap-vcpkg.bat` |
|  Unix   | `./vcpkg/bootstrap-vcpkg.sh`  |

### Build the project

Now, we can generate the build files. You don't have to use Ninja, but it is highly recommended. On all operating
systems, you should use a separate build directory where all of our generated build files and (later) binaries will end
up.

```bash
mkdir build
cd build
```

Use the following commands to tell CMake to generate the build files and subsequently build the project. Note, if you
are using an IDE, such as JetBrains CLion, you can specify CMake parameters in your CMake profile settings so that you
can just build the project as per usual. In CLion 2021.2, you'll access these settings
in `View -> Tool Windows -> CMake`, and then selecting `CMake Settings`. You can also just search for `CMake Settings`
with `Shift+Shift`. Subsequently, in the `CMake Settings` window, add the parameters to the `CMake options` field.
Specify a custom Vcpkg triplet by providing a `VCPKG_TARGET_TRIPLET` CMake parameter.

```bash
cmake .. -GNinja
ninja
```
