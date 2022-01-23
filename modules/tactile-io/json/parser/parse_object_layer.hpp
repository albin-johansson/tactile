#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const JSON& json, Layer& layer) -> ParseError;

[[nodiscard]] auto ParseObjectLayer(const JSON& json, Tile& tile) -> ParseError;

}  // namespace tactile::IO
