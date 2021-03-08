# tactile [![version](https://img.shields.io/badge/version-0.2.0-blue.svg)](https://semver.org) [![Language](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Build status](https://ci.appveyor.com/api/projects/status/1dsfluq6ep8yj5lr?svg=true)](https://ci.appveyor.com/project/AlbinJohansson/tactile)

A tilemap editor that aims to be easy-to-use, fast and lightweight. Furthermore, compatibility with
the excellent [Tiled](https://www.mapeditor.org/) map editor is pursued.

![example](meta/screenshots/v020.png "example")

## Aim

The following bullet points describe the goals that guide the design and development of the Tactile tilemap editor.

* Must be easy to learn and get started with.
* Must be easy to use.
* Keep things simple, hide advanced options that are rarely used.
* Well documented and high-quality source code.

## Source code

The Tactile application is written in C++20 using MSVC. At the time of writing, the code using features that are not yet available in stable releases of GCC and Clang, which means that Tactile is only available on Windows, for now. However, with time any C++20 compatible compiler should be able to build the sources.
