#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const JSON& json, Map& map) -> ParseError;

}  // namespace Tactile::IO
