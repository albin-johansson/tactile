#pragma once

#include <expected.hpp>

#include "../parse_error.hpp"
#include "../parse_ir.hpp"
#include "yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObjectLayer(const YAML::Node& node)
    -> tl::expected<ObjectLayerData, ParseError>;

}  // namespace Tactile::IO
