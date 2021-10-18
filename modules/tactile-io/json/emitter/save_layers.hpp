#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../../parse_ir.hpp"
#include "../json_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveLayers(const std::vector<LayerData>& layers,
                              const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO
