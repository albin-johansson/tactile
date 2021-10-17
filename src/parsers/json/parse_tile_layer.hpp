#pragma once

#include "common/expected.hpp"
#include "io/json.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const JSON& json)
    -> Expected<TileLayerData, ParseError>;

}  // namespace Tactile::IO
