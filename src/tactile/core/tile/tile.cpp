// Copyright (C) 2025 Albin Johansson

module tactile.core.tile;

import tactile.core.prelude;

namespace tactile {

Tile::Tile(const TileID id)
  : m_id {id}
{}

auto Tile::id() const -> TileID
{
  return m_id;
}

auto Tile::animation() -> Option<TileAnimation>&
{
  return m_animation;
}

auto Tile::animation() const -> const Option<TileAnimation>&
{
  return m_animation;
}

}  // namespace tactile
