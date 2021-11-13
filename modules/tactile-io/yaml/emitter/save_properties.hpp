#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../yaml_fwd.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

void SaveProperties(YAML::Emitter& emitter,
                    const Map& map,
                    const std::filesystem::path& dir);

void SaveProperties(YAML::Emitter& emitter,
                    const Layer& layer,
                    const std::filesystem::path& dir);

void SaveProperties(YAML::Emitter& emitter,
                    const Tileset& tileset,
                    const std::filesystem::path& dir);

void SaveProperties(YAML::Emitter& emitter,
                    const Tile& tile,
                    const std::filesystem::path& dir);

void SaveProperties(YAML::Emitter& emitter,
                    const Object& object,
                    const std::filesystem::path& dir);

}  // namespace Tactile::IO