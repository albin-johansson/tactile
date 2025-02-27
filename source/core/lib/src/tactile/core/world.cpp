// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/world.hpp"

#include "tactile/core/world_data.hpp"

namespace tactile {

World::World()
  : m_data {make_unique<WorldData>()}
{}

World::~World() noexcept = default;

auto World::get_data() -> WorldData&
{
  return *m_data;
}

auto World::get_data() const -> const WorldData&
{
  return *m_data;
}

}  // namespace tactile
