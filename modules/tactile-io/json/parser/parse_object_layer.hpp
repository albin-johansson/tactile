#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const JSON& json, Layer& layer) -> ParseError;

[[nodiscard]] auto ParseObjectLayer(const JSON& json, Tile& tile) -> ParseError;

}  // namespace Tactile::IO
