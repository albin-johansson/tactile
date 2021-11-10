#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry, entity

namespace Tactile::Sys {

[[nodiscard]] auto FindObject(const entt::registry& registry, ObjectID id)
    -> entt::entity;

}  // namespace Tactile::Sys