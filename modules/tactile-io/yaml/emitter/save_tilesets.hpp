#pragma once

#include <filesystem>  // path

#include "../yaml_fwd.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

void SaveTilesets(YAML::Emitter& emitter,
                  const Map& map,
                  const std::filesystem::path& dir);

}  // namespace Tactile::IO
