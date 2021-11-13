#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const JSON& json, Layer& layer) -> ParseError;

[[nodiscard]] auto ParseObjectLayer(const JSON& json, Tile& tile) -> ParseError;

}  // namespace Tactile::IO
