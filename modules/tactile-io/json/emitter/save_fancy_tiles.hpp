#pragma once

#include <filesystem>  // path

#include "../../parse_ir.hpp"
#include "../json_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveFancyTiles(const TilesetData& tileset,
                                  const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO