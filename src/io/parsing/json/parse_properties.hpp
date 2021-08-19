#pragma once

#include <json.hpp>  // json
#include <vector>    // vector

#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const nlohmann::json& json,
                                   std::vector<PropertyData>& properties)
    -> ParseError;

}  // namespace Tactile::IO
