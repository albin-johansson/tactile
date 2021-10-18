#pragma once

#include <vector>  // vector

#include <expected.hpp>  // expected

#include "../parse_error.hpp"
#include "../parse_ir.hpp"
#include "yaml_fwd.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const YAML::Node& node)
    -> tl::expected<std::vector<PropertyData>, ParseError>;

}  // namespace Tactile::IO