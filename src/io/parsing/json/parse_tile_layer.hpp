#pragma once

#include "common/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const JSON& json, LayerData& layer) -> ParseError;

}  // namespace Tactile::IO
