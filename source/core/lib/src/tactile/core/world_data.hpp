// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // same_as

#include <entt/entity/registry.hpp>

#include "tactile/core/entity/entity_id.hpp"
#include "tactile/core/world.hpp"

namespace tactile {

static_assert(std::same_as<EntityID, entt::id_type>);

/// Provides the internal world data.
class WorldData final
{
 public:
  /// Returns the associated ECS registry.
  [[nodiscard]]
  auto get_registry() -> entt::registry&;

  /// Returns the associated ECS registry.
  [[nodiscard]]
  auto get_registry() const -> const entt::registry&;

 private:
  entt::registry m_registry {};
};

}  // namespace tactile
