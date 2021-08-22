#pragma once

#include <vector>  // vector

#include "aliases/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const JSON& json,
                                   std::vector<PropertyData>& properties)
    -> ParseError;

}  // namespace Tactile::IO
