# Tactile IO

This module contains the source code for the Tactile "IO" framework, which is built as a standalone
shared library. The idea is that this library can be used by applications that want to either parse
or emit Tactile (or Tiled) map files.

## Design

This library has been designed with ABI stability in mind, so the exported APIs do not make use of
any STL types (except for inline convenience functions). Subsequently, the IR types are only
forward-declared and only used by reference, to keep their definitions hidden in client code.
Overall, this arguably has a negative effect on the elegance of the APIs, but it is unfortunately
necessary to guarantee ABI stability. However, usage of the APIs can be made slightly less wordy by utilizing ADL (argument-dependent
lookup), as shown in the following example.

```C++
auto irMap = Tactile::IO::CreateMap();

auto& foo = Tactile::IO::AddLayer(*irMap); // Full function qualification 
auto& bar = AddLayer(*irMap);              // Makes use of ADL
```

## Installation

In order to make use of this library in an external application, you will need to include the public
headers, located in the `include` folder. Furthermore, you will need to include the Tactile base
headers, which are provided in the `tactile-base` "module".

After you've copied the public headers, you'll need to download the runtime binaries, which are
included in official Tactile releases. You can find the latest release
[here](https://github.com/albin-johansson/tactile/releases).

## Usage

Once you've managed to successfully install the library, you'll be able to parse and emit maps as in
the following example.

```C++
#include <tactile-io/tactile_io.hpp>    // Includes all public Tactile IO headers 

#include <iostream>  // cout

void Emit() 
{
  auto irMap = Tactile::IO::CreateMap();
  Tactile::IO::SetPath(*irMap, "resources/foo.yaml");

  Tactile::IO::SetRowCount(*irMap, 42);
  Tactile::IO::SetColumnCount(*irMap, 123);
  
  auto& layer = Tactile::IO::AddLayer(*irMap);
  Tactile::IO::SetName(layer, "Foo");

  // ...

  Tactile::IO::EmitYamlMap(*irMap, Tactile::IO::EmitterOptions_FoldTileData);
}

void Parse()
{
  Tactile::IO::ParseError err{};
  if (const auto irMap = Tactile::IO::ParseMap("resources/foo.yaml", &err)) {
    const auto rows = Tactile::IO::GetRowCount(*irMap);
    const auto columns = Tactile::IO::GetColumnCount(*irMap);
    
    // ...
  }
  else {
    std::cout << "Failed to parse map: " << Tactile::IO::GetCause(err) << '\n';
  }
}
```