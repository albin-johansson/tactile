/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

namespace tactile {

Object::Object(const ObjectType type)
    : mType {type}
{
}

void Object::accept(ContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void Object::set_pos(const Float2& pos)
{
  mPos = pos;
}

void Object::set_size(const Float2& size)
{
  mSize = size;
}

void Object::set_type(const ObjectType type)
{
  mType = type;
  if (is_point()) {
    mSize.x = 0;
    mSize.y = 0;
  }
}

void Object::set_tag(String tag)
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

auto Object::get_ctx() -> ContextInfo&
{
  return mContext;
}

auto Object::get_ctx() const -> const ContextInfo&
{
  return mContext;
}

auto Object::get_uuid() const -> const UUID&
{
  return mContext.get_uuid();
}

auto Object::get_type() const -> ObjectType
{
  return mType;
}

auto Object::is_rect() const -> bool
{
  return mType == ObjectType::Rect;
}

auto Object::is_ellipse() const -> bool
{
  return mType == ObjectType::Ellipse;
}

auto Object::is_point() const -> bool
{
  return mType == ObjectType::Point;
}

auto Object::get_pos() const -> const Float2&
{
  return mPos;
}

auto Object::get_size() const -> const Float2&
{
  return mSize;
}

auto Object::get_tag() const -> const String&
{
  return mTag;
}

auto Object::get_meta_id() const -> Maybe<int32>
{
  return mMetaId;
}

auto Object::is_visible() const -> bool
{
  return mVisible;
}

auto Object::clone() const -> Shared<Object>
{
  auto copy = std::make_shared<Object>(*this);
  copy->mContext = mContext.clone();

  return copy;
}

}  // namespace tactile