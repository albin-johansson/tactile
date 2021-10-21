#pragma once

#include <expected.hpp>  // expected

#include "../yaml_fwd.hpp"
#include "parse_error.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseObject(const YAML::Node& node)
    -> tl::expected<ObjectData, ParseError>;

}  // namespace Tactile::IO