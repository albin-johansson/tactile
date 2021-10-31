#pragma once

#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry, entity

namespace Tactile::Sys {

[[nodiscard]] auto FindObject(const entt::registry& registry, ObjectID id)
    -> entt::entity;

}  // namespace Tactile::Sys