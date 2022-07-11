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

#include "object.hpp"

#include <utility>  // move

#include "core/contexts/context_visitor.hpp"

namespace tactile {

void Object::set_pos(const Vector2f& pos)
{
  mPos = pos;
}

void Object::set_size(const Vector2f& size)
{
  mSize = size;
}

void Object::set_type(const ObjectType type)
{
  mType = type;
}

void Object::set_tag(std::string tag)
{
  mTag = std::move(tag);
}

void Object::set_meta_id(const int32 id)
{
  mMetaId = id;
}

void Object::set_visible(const bool visible)
{
  mVisible = visible;
}

void Object::set_name(std::string name)
{
  mDelegate.set_name(std::move(name));
}

void Object::accept(IContextVisitor& visitor) const
{
  visitor.visit(*this);
}

auto Object::get_props() -> PropertyBundle&
{
  return mDelegate.get_props();
}

auto Object::get_props() const -> const PropertyBundle&
{
  return mDelegate.get_props();
}

auto Object::get_comps() -> ComponentBundle&
{
  return mDelegate.get_comps();
}

auto Object::get_comps() const -> const ComponentBundle&
{
  return mDelegate.get_comps();
}

auto Object::get_uuid() const -> const UUID&
{
  return mDelegate.get_uuid();
}

auto Object::get_name() const -> const std::string&
{
  return mDelegate.get_name();
}

}  // namespace tactile