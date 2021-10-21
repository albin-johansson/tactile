#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../yaml_fwd.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

void SaveTilesets(YAML::Emitter& emitter,
                  const std::vector<TilesetData>& tilesets,
                  const std::filesystem::path& dir);

}  // namespace Tactile::IO
