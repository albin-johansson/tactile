#pragma once

#include <nlohmann/json_fwd.hpp>

#include "io/maps/ir.hpp"
#include "io/maps/parser/parse_error.hpp"

namespace tactile::parsing {

[[nodiscard]] auto parse_properties(const nlohmann::json& json,
                                    ir::attribute_context_data& contextData)
    -> parse_error;

}  // namespace tactile::parsing