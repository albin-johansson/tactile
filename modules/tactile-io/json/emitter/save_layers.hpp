#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../json_common.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveLayers(const std::vector<LayerData>& layers,
                              const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO
