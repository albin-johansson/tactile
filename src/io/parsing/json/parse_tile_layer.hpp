#pragma once

#include "common/expected.hpp"
#include "common/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const JSON& json)
    -> Expected<TileLayerData, ParseError>;

}  // namespace Tactile::IO
