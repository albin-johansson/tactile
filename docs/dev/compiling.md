# Compiling

This document provides a guide for how to build the project.

- [Compiling](#compiling)
  - [Install a C++ compiler](#install-a-c-compiler)
  - [Install Vcpkg](#install-vcpkg)
  - [Building the project](#building-the-project)
  - [Configuring profiles in JetBrains CLion](#configuring-profiles-in-jetbrains-clion)

Tactile uses [Vcpkg](https://github.com/microsoft/vcpkg), an open-source dependency manager for C++ libraries, developed by Microsoft.
This makes building the Tactile editor really quite straightforward.

## Install a C++ compiler

Tactile is written in a subset of C++23, which has good support by the major compilers by now.
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

Furthermore, Tactile must be built using a custom Vcpkg "triplet", which controls aspects such as whether dependencies are built as static or dynamic libraries.
The `add_vcpkg_triplet.py` script can be used to automatically register the appropriate triplet file for your system.
More information about Vcpkg triplets can be found [here](https://github.com/microsoft/vcpkg/docs/users/triplets.md).

```bash
> cd scripts
> python add_vcpkg_triplet.py
```

## Building the project

Given a successful Vcpkg installation, building the project should be a simple as entering the following commands, starting in the root directory of the repository.

It is advisable to make use of the Ninja generator, since that improves compile times significantly.
However, you can specify whatever generator you want (and omit the `-G` argument to use the default generator).

```bash
> mkdir build
> cd build
> cmake .. -DVCPKG_TARGET_TRIPLET=<tactile-triplet> -DCMAKE_BUILD_TYPE=Debug -GNinja
> ninja
```

Remember, `<tactile-triplet>` must correspond to one of the custom Tactile triplets, e.g. `arm64-osx-tactile` for M1 macs or `x64-windows-tactile` for x86_64 Windows machines.

## Install asset files

While asset files such as fonts are not required during compilation, they are required at runtime.
The required asset files can be installed by simply running the `install_assets.py` script.

```bash
> cd scripts
> python install_assets.py
```

## Configuring profiles in JetBrains CLion

If you're using an IDE such as JetBrains CLion, it is usually possible to configure CMake profiles to make building and/or running the project as simple as pressing a button.

You can configure CMake profiles in the project settings under `Build, Execution, Deployment` -> `CMake`.
You can also simply search for `CMake Settings` using the `Shift+Shift` shortcut.
Here, you'll be able to configure CMake arguments such as `VCPKG_TARGET_TRIPLET`, the preferred generator, and the build type of a profile.
