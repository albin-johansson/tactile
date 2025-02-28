// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/world_data.hpp"

namespace tactile {

auto WorldData::get_registry() -> entt::registry&
{
  return m_registry;
}

auto WorldData::get_registry() const -> const entt::registry&
{
  return m_registry;
}

}  // namespace tactile
