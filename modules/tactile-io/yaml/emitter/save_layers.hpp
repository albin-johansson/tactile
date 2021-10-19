#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../../parse_ir.hpp"
#include "../yaml_fwd.hpp"

namespace Tactile::IO {

void SaveLayers(YAML::Emitter& emitter,
                const std::vector<LayerData>& layers,
                const std::filesystem::path& dir);

}  // namespace Tactile::IO
