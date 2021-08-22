#pragma once

#include <filesystem>  // path

#include "aliases/json.hpp"
#include "core/map/map.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveLayers(const Map& map, const std::filesystem::path& dir)
    -> JSON;

}  // namespace Tactile::IO
