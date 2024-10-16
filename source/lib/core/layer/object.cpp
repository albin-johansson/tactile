// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

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
    mSize = Float2 {0, 0};
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
