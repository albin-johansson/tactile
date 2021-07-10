#pragma once

#include <vector>  // vector

#include "io/parsing/map_file.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const IMapObject& object,
                                   std::vector<PropertyData>& properties)
    -> ParseError;

}  // namespace Tactile::IO
