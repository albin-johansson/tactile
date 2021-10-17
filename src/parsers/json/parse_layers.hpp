#pragma once

#include <vector>  // vector

#include "common/expected.hpp"
#include "io/json.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const JSON& json)
    -> Expected<std::vector<LayerData>, ParseError>;

}  // namespace Tactile::IO
