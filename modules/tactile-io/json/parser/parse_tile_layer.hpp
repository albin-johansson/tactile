#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const JSON& json, Layer& layer) -> ParseError;

}  // namespace Tactile::IO
