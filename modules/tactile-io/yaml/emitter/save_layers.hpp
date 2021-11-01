#pragma once

#include <filesystem>  // path

#include "../yaml_fwd.hpp"
#include "ir.hpp"

namespace Tactile::IO {

void SaveLayers(YAML::Emitter& emitter,
                const Map& map,
                const std::filesystem::path& dir,
                uint32 options);

}  // namespace Tactile::IO
