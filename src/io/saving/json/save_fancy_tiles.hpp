#pragma once

#include <entt.hpp>    // registry
#include <filesystem>  // path

#include "core/components/tileset.hpp"
#include "io/json.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveFancyTiles(const entt::registry& registry,
                                  const Tileset& tileset,
                                  const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO