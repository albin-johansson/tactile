#pragma once

#include <filesystem>  // path
#include <vector>      // vector

#include "../json_common.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

struct EmitterOptions;

[[nodiscard]] auto SaveTilesets(const std::vector<TilesetData>& tilesets,
                                const std::filesystem::path& dir,
                                const EmitterOptions& options) -> JSON;

}  // namespace Tactile::IO
