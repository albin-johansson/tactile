# Compiling

This document provides a guide for how to build the project.

- [Compiling](#compiling)
  - [Install a C++ compiler](#install-a-c-compiler)
  - [Install Vcpkg](#install-vcpkg)
  - [Building the project](#building-the-project)

Tactile uses [Vcpkg](https://github.com/microsoft/vcpkg), an open-source dependency manager for C++ libraries, developed by Microsoft.
This makes building the Tactile editor really quite straightforward.

## Install a C++ compiler

On both Windows and macOS, you can install a C++ compiler by installing their primary IDEs, i.e. Visual Studio and Xcode.
For Linux, you should use your distributions package manager, e.g. `sudo apt install g++`.

## Install Vcpkg

The process of installing Vcpkg really just boils down to cloning a GitHub repository, running a configuration script, and setting an environment variable.

Enter the following commands in your shell in the directory you'd like to install Vcpkg.
Note, the below example works on Unix systems. On Windows, you probably need to run `.\bootstrap-vcpkg.bat -disableMetrics` instead.
The `-disableMetrics` flag is optional.

```bash
> git clone https://github.com/microsoft/vcpkg
> cd vcpkg
> ./bootstrap-vcpkg.sh -disableMetrics
```

It is recommended to set the environment variable `VCPKG_ROOT` to point to the directory where you installed Vcpkg, this will make your CMake build command simpler and less error-prone.

```bash
> echo $VCPKG_ROOT  # Possible output: '/Users/steve/vcpkg'
```

## Building the project

Given a successful Vcpkg installation, building the project should be a simple as entering the following commands, starting in the root directory of the repository.
Use the correct preset in the `CMakePresets.json` for your system.
The following works for ARM-based macOS systems.

```bash
> mkdir build
> cd build
> cmake .. --preset arm64-osx-homebrew-llvm-debug
> ninja
```

Depending on the version of CMake you're using, it might be necessary to override the value of
`CMAKE_EXPERIMENTAL_CXX_IMPORT_STD`, see [this](https://github.com/Kitware/CMake/blob/master/Help/dev/experimental.rst)
page for the correct value.
