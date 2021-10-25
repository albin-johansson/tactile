# Source code

This document provides an overview of the structure of the Tactile source code. The source code of
third-party dependencies are provided in the `lib` folder. However, some dependencies are not part
of the repository but instead downloaded at build time using CMake's `FetchContent` module.

## tactile-io

This folder contains the sources for the YAML/JSON/XML parsers and emitters, and can be used in
third-party applications as a shared library dependency.

```C++
#include <tactile-io/parser.hpp>

int main()
{
  Tactile::IO::ParseError error{};
  if (const auto map = Tactile::IO::ParseMap("foo.yaml", &error)) {
    // ...
  }
}
```