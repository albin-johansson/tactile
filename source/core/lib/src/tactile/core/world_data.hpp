// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <type_traits>  // is_same_v

#include <entt/entity/registry.hpp>

#include "tactile/core/world.hpp"

namespace tactile {

static_assert(std::is_same_v<EntityID, entt::id_type>);

class WorldData final
{
 private:
  entt::registry m_registry {};
};

}  // namespace tactile
