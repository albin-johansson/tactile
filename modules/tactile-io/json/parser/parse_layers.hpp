#pragma once

#include "../json_common.hpp"
#include "ir.hpp"
#include "parse_error.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const JSON& json, Map& map) -> ParseError;

}  // namespace Tactile::IO
