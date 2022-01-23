#pragma once

#include <filesystem>  // path

#include "../yaml_fwd.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

void SaveLayers(YAML::Emitter& emitter,
                const Map& map,
                const std::filesystem::path& dir,
                EmitterOptions options);

}  // namespace tactile::IO
