// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/level.hpp"

#include "tactile/core/level_data.hpp"

namespace tactile {

Level::Level()
  : m_data {make_unique<LevelData>()}
{}

Level::~Level() noexcept = default;

void Level::visit(ILevelVisitor& visitor) const
{
  // TODO
}

void Level::visit(ILevelRootLayerVisitor& visitor) const
{
  // TODO
}

auto Level::get_data() -> LevelData&
{
  return *m_data;
}

auto Level::get_data() const -> const LevelData&
{
  return *m_data;
}

}  // namespace tactile
