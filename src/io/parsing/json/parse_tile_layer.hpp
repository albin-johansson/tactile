#pragma once

#include <json.hpp>  // json

#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseTileLayer(const nlohmann::json& json, LayerData& layer)
    -> ParseError;

}  // namespace Tactile::IO
