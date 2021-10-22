#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveFancyTiles(const Tileset& tileset,
                                  const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO