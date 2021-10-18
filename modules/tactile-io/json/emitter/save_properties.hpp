#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../../parse_ir.hpp"
#include "../json_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveProperties(const std::vector<PropertyData>& properties,
                                  const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO
