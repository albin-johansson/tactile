#pragma once

#include <expected.hpp>  // expected

#include "../json_common.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const JSON& json)
    -> tl::expected<ObjectLayerData, ParseError>;

}  // namespace Tactile::IO
