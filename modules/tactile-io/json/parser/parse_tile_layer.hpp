#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const JSON& json, Layer& layer) -> ParseError;

}  // namespace Tactile::IO
