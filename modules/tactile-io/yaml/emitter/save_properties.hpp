#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../../parse_ir.hpp"
#include "../yaml_fwd.hpp"

namespace Tactile::IO {

void SaveProperties(YAML::Emitter& emitter,
                    const std::vector<PropertyData>& properties,
                    const std::filesystem::path& dir);

}  // namespace Tactile::IO