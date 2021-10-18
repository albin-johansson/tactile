#pragma once

#include <tactile-io/parse_error.hpp>
#include <tactile-io/parse_ir.hpp>

#include "common/expected.hpp"
#include "io/json.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const JSON& json)
    -> Expected<ObjectLayerData, ParseError>;

}  // namespace Tactile::IO
