#pragma once

#include "io/parsing/map_file.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const IMapFile& file, MapData& data) -> ParseError;

}  // namespace Tactile::IO
