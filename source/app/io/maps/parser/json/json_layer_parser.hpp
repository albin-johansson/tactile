#pragma once

#include <nlohmann/json_fwd.hpp>

#include "io/maps/ir.hpp"
#include "io/maps/parser/parse_error.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_object(const nlohmann::json& json, ir::object_data& objectData)
    -> parse_error;

[[nodiscard]] auto parse_layers(const nlohmann::json& json, ir::map_data& mapData)
    -> parse_error;

}  // namespace tactile::parsing
