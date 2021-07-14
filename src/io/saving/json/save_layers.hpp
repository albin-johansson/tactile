#pragma once

#include <filesystem>  // path
#include <json.hpp>    // json

#include "core/map/map.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveLayers(const Map& map, const std::filesystem::path& dir)
    -> nlohmann::json;

}  // namespace Tactile::IO
