#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "tactile_io.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveProperties(const Map& map, const std::filesystem::path& dir)
    -> JSON;

[[nodiscard]] auto SaveProperties(const Layer& layer, const std::filesystem::path& dir)
    -> JSON;

[[nodiscard]] auto SaveProperties(const Tileset& tileset,
                                  const std::filesystem::path& dir) -> JSON;

[[nodiscard]] auto SaveProperties(const Tile& tile, const std::filesystem::path& dir)
    -> JSON;

[[nodiscard]] auto SaveProperties(const Object& object, const std::filesystem::path& dir)
    -> JSON;

}  // namespace Tactile::IO
