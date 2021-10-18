#pragma once

#include <vector>  // vector

#include <expected.hpp>  // expected

#include "../../parse_error.hpp"
#include "../../parse_ir.hpp"
#include "../json_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseProperties(const JSON& json)
    -> tl::expected<std::vector<PropertyData>, ParseError>;

}  // namespace Tactile::IO
