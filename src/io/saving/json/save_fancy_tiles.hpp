#pragma once

#include <entt.hpp>    // registry
#include <filesystem>  // path

#include "common/json.hpp"
#include "core/components/tileset.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveFancyTiles(const entt::registry& registry,
                                  const Tileset& tileset,
                                  const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO