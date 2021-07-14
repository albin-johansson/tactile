#pragma once

#include <filesystem>  // path
#include <json.hpp>    // json

#include "core/map_document.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveTilesets(const MapDocument& document,
                                const std::filesystem::path& dir) -> nlohmann::json;

}  // namespace Tactile::IO
