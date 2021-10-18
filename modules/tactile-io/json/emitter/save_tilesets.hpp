#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../../parse_ir.hpp"
#include "../json_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveTilesets(const std::vector<TilesetData>& tilesets,
                                const std::filesystem::path& dir,
                                bool embed) -> JSON;

}  // namespace Tactile::IO
