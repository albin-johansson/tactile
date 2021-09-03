#pragma once

#include <entt.hpp>  // registry, entity

#include "aliases/object_id.hpp"

namespace Tactile::Sys {

/// \name Object layer system
/// \{

[[nodiscard]] auto HasObject(const entt::registry& registry,
                             entt::entity entity,
                             object_id id) -> bool;

[[nodiscard]] auto FindObject(const entt::registry& registry,
                              entt::entity entity,
                              object_id id) -> entt::entity;

/// \} End of object layer system

}  // namespace Tactile::Sys
