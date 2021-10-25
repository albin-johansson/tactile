#pragma once

#include "../json_common.hpp"
#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const JSON& json, Layer& layer) -> ParseError;

}  // namespace Tactile::IO
