#pragma once

#include "common/expected.hpp"
#include "io/json.hpp"
#include "parsers/parse_error.hpp"
#include "parsers/parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const JSON& json)
    -> Expected<ObjectLayerData, ParseError>;

}  // namespace Tactile::IO
