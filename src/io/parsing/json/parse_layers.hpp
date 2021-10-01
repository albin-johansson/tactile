#pragma once

#include <vector>  // vector

#include "common/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const JSON& json, std::vector<LayerData>& layers)
    -> ParseError;

}  // namespace Tactile::IO
