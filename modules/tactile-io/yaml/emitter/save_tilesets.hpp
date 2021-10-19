#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "parse_ir.hpp"
#include "../yaml_fwd.hpp"

namespace Tactile::IO {

void SaveTilesets(YAML::Emitter& emitter,
                  const std::vector<TilesetData>& tilesets,
                  const std::filesystem::path& dir);

}  // namespace Tactile::IO
