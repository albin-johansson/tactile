#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "tactile_io.hpp"

namespace tactile::IO {

[[nodiscard]] auto SaveTilesets(const Map& map,
                                const std::filesystem::path& dir,
                                EmitterOptions options) -> JSON;

}  // namespace tactile::IO
