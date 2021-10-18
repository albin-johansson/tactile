#pragma once

#include <vector>  // vector

#include <tactile/io/maps/parse_error.hpp>
#include <tactile/io/maps/parse_ir.hpp>

#include "common/expected.hpp"
#include "io/json.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseLayers(const JSON& json)
    -> Expected<std::vector<LayerData>, ParseError>;

}  // namespace Tactile::IO
