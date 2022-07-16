/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tile.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "core/ctx/context_visitor.hpp"
#include "misc/panic.hpp"

namespace tactile {

Tile::Tile(const TileIndex index)
    : mIndex {index}
{
  mDelegate.set_name(fmt::format("Tile {}", index));
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

void Tile::accept(IContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void Tile::set_name(std::string name)
{
  mDelegate.set_name(std::move(name));
}

void Tile::set_source(const Vector4i& source)
{
  mSource = source;
}

auto Tile::object_count() const -> usize
{
  return mObjects.size();
}

auto Tile::object_capacity() const -> usize
{
  return mObjects.bucket_count();
}

auto Tile::get_objects() const -> const HashMap<UUID, Shared<Object>>&
{
  return mObjects;
}

auto Tile::is_animated() const -> bool
{
  return mAnimation.has_value();
}

auto Tile::get_animation() const -> const TileAnimation&
{
  if (mAnimation) {
    return *mAnimation;
  }
  else {
    throw TactileError {"Tile is not animated!"};
  }
}

auto Tile::get_props() -> PropertyBundle&
{
  return mDelegate.get_props();
}

auto Tile::get_props() const -> const PropertyBundle&
{
  return mDelegate.get_props();
}

auto Tile::get_comps() -> ComponentBundle&
{
  return mDelegate.get_comps();
}

auto Tile::get_comps() const -> const ComponentBundle&
{
  return mDelegate.get_comps();
}

auto Tile::get_uuid() const -> const UUID&
{
  return mDelegate.get_uuid();
}

auto Tile::get_name() const -> const std::string&
{
  return mDelegate.get_name();
}

}  // namespace tactile