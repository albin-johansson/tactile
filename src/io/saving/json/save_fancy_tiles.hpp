#pragma once

#include <entt.hpp>    // registry, entity
#include <filesystem>  // path

#include "aliases/json.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveFancyTiles(const entt::registry& registry,
                                  entt::entity tilesetEntity,
                                  const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO