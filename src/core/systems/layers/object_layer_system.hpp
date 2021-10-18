#pragma once

#include <entt.hpp>  // registry, entity

#include <tactile-base/tactile_std.hpp>

namespace Tactile::Sys {

/// \name Object layer system
/// \{

[[nodiscard]] auto HasObject(const entt::registry& registry,
                             entt::entity entity,
                             ObjectID id) -> bool;

[[nodiscard]] auto FindObject(const entt::registry& registry,
                              entt::entity entity,
                              ObjectID id) -> entt::entity;

/// \} End of object layer system

}  // namespace Tactile::Sys
