#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../yaml_fwd.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

void SaveProperties(YAML::Emitter& emitter,
                    const std::vector<PropertyData>& properties,
                    const std::filesystem::path& dir);

}  // namespace Tactile::IO