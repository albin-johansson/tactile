#pragma once

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseLayers(const JSON& json, Map& map) -> ParseError;

}  // namespace tactile::IO
