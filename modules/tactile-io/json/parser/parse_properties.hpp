#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const JSON& json, Map& map) -> ParseError;

[[nodiscard]] auto ParseProperties(const JSON& json, Tileset& tileset) -> ParseError;

[[nodiscard]] auto ParseProperties(const JSON& json, Tile& tile) -> ParseError;

[[nodiscard]] auto ParseProperties(const JSON& json, Layer& layer) -> ParseError;

[[nodiscard]] auto ParseProperties(const JSON& json, Object& object) -> ParseError;

}  // namespace Tactile::IO
