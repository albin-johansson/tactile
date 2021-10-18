#pragma once

#include <expected.hpp>  // expected

#include "../../parse_error.hpp"
#include "../../parse_ir.hpp"
#include "../json_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const JSON& json)
    -> tl::expected<TileLayerData, ParseError>;

}  // namespace Tactile::IO
