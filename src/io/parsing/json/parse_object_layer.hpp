#pragma once

#include "common/expected.hpp"
#include "common/json.hpp"
#include "io/parsing/parse_error.hpp"
#include "io/parsing/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const JSON& json)
    -> Expected<ObjectLayerData, ParseError>;

}  // namespace Tactile::IO
