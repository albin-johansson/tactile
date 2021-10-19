#pragma once

#include "api.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

// TODO pass along options such as whether tilesets are embedded

TACTILE_IO_API void EmitJsonMap(const MapData& data);

TACTILE_IO_API void EmitXmlMap(const MapData& data);

TACTILE_IO_API void EmitYamlMap(const MapData& data);

}  // namespace Tactile::IO