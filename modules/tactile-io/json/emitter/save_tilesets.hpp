#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveTilesets(const Map& map,
                                const std::filesystem::path& dir,
                                uint32 options) -> JSON;

}  // namespace Tactile::IO
