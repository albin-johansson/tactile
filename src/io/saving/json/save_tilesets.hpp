#pragma once

#include <filesystem>  // path

#include "aliases/json.hpp"
#include "core/map_document.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveTilesets(const MapDocument& document,
                                const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO
