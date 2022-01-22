#pragma once

#include <filesystem>  // path

#include "../yaml_fwd.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

void SaveComponentDefinitions(YAML::Emitter& emitter,
                              const Map& map,
                              const std::filesystem::path& dir);

void SaveComponents(YAML::Emitter& emitter,
                    const Map& map,
                    const std::filesystem::path& dir);

void SaveComponents(YAML::Emitter& emitter,
                    const Layer& layer,
                    const std::filesystem::path& dir);

void SaveComponents(YAML::Emitter& emitter,
                    const Object& object,
                    const std::filesystem::path& dir);

void SaveComponents(YAML::Emitter& emitter,
                    const Tileset& tileset,
                    const std::filesystem::path& dir);

void SaveComponents(YAML::Emitter& emitter,
                    const Tile& tile,
                    const std::filesystem::path& dir);

}  // namespace Tactile::IO
