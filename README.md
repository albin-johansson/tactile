# Tactile

[![License](https://img.shields.io/badge/license-GPL3-blue.svg)](https://opensource.org/licenses/GPL-3.0)
[![version](https://img.shields.io/github/v/release/albin-johansson/tactile)](https://github.com/albin-johansson/tactile/releases)
[![CI: Windows](https://github.com/albin-johansson/tactile/actions/workflows/windows.yml/badge.svg?branch=dev)](https://github.com/albin-johansson/tactile/actions/workflows/windows.yml)

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
* Theme editor
* Minimap dock
* Components
* Tileset editor

## Dependencies

* [Centurion](https://github.com/albin-johansson/centurion)
* [Dear ImGui](https://github.com/ocornut/imgui)
* [EnTT](https://github.com/skypjack/entt)
* [fmt](https://github.com/fmtlib/fmt)
* [GLEW](https://github.com/nigels-com/glew)
* [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders)
* [JSON for Modern C++](https://github.com/nlohmann/json)
* [Magic Enum C++](https://github.com/Neargye/magic_enum)
* [Nenya](https://github.com/albin-johansson/nenya)
* [Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs)
* [pugixml](https://github.com/zeux/pugixml)
* [Rune](https://github.com/albin-johansson/rune)
* [SDL2](https://github.com/libsdl-org/SDL)
* [SDL2_image](https://github.com/libsdl-org/SDL_image)
* [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
* [stb_image](https://github.com/nothings/stb)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)
