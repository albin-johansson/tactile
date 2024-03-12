// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tile.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "tactile/core/debug/exception.hpp"

namespace tactile {

Tile::Tile(const TileIndex index)
    : mIndex {index}
{
  mContext.set_name(fmt::format("Tile {}", index));
}

void Tile::accept(ContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void Tile::update()
{
  if (mAnimation) {
    mAnimation->update();
  }
}

void Tile::reserve_objects(usize n)
{
  mObjects.reserve(n);
}

void Tile::add_object(Shared<Object> object)
{
  const auto id = object->get_uuid();
  mObjects[id] = std::move(object);
}

void Tile::clear_animation()
{
  mAnimation.reset();
}

void Tile::set_animation(TileAnimation animation)
{
  mAnimation = std::move(animation);
}

void Tile::set_source(const Int4& source)
{
  mSource = source;
}

auto Tile::object_count() const -> usize
{
  return mObjects.size();
}

auto Tile::get_objects() const -> const ObjectMap&
{
  return mObjects;
}

auto Tile::is_animated() const -> bool
{
  return mAnimation.has_value();
}

auto Tile::get_animation() -> TileAnimation&
{
  if (mAnimation) {
    return *mAnimation;
  }
  else {
    throw Exception {"No animation associated with tile"};
  }
}

auto Tile::get_animation() const -> const TileAnimation&
{
  if (mAnimation) {
    return *mAnimation;
  }
  else {
    throw Exception {"No animation associated with tile"};
  }
}

auto Tile::get_ctx() -> ContextInfo&
{
  return mContext;
}

auto Tile::get_ctx() const -> const ContextInfo&
{
  return mContext;
}

auto Tile::get_uuid() const -> const UUID&
{
  return mContext.get_uuid();
}

}  // namespace tactile
