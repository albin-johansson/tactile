#pragma once

#include <entt.hpp>    // registry
#include <filesystem>  // path

#include "io/json.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveTilesets(const entt::registry& registry,
                                const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO
