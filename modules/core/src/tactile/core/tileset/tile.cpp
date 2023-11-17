// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/tile.hpp"

#include <utility>  // move

namespace tactile {

Tile::Tile(const Rectangle& region)
  : mRegion {region}
{}

void Tile::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);
}

void Tile::set_animation(Maybe<TileAnimation> animation)
{
  mAnimation = std::move(animation);
}

auto Tile::get_meta() -> Metadata&
{
  return mMeta;
}

auto Tile::get_meta() const -> const Metadata&
{
  return mMeta;
}

auto Tile::get_region() const -> const Rectangle&
{
  return mRegion;
}

auto Tile::get_animation() -> TileAnimation&
{
  return mAnimation.value();
}

auto Tile::get_animation() const -> const TileAnimation&
{
  return mAnimation.value();
}

auto Tile::is_animated() const -> bool
{
  return mAnimation.has_value();
}

}  // namespace tactile
