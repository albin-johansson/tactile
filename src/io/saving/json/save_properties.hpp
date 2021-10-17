#pragma once

#include <entt.hpp>    // registry, entity
#include <filesystem>  // path

#include "io/json.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveProperties(const entt::registry& registry,
                                  entt::entity entity,
                                  const std::filesystem::path& dir) -> JSON;

}  // namespace Tactile::IO
