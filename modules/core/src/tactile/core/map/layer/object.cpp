// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/object.hpp"

#include <utility>  // move

namespace tactile {

Object::Object(const ObjectType type)
  : mType {type}
{}

void Object::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);
}

void Object::set_type(const ObjectType type)
{
  mType = type;
}

void Object::set_position(const Float2 position)
{
  mPosition = position;
}

void Object::set_size(const Float2 size)
{
  mSize = size;
}

void Object::set_tag(String tag)
{
  mTag = std::move(tag);
}

void Object::set_persistent_id(const Maybe<int32> id)
{
  mPersistentId = id;
}

void Object::set_visible(const bool visible)
{
  mVisible = visible;
}

auto Object::get_type() const -> ObjectType
{
  return mType;
}

auto Object::get_position() const -> Float2
{
  return mPosition;
}

auto Object::get_size() const -> Float2
{
  return mSize;
}

auto Object::get_tag() const -> const String&
{
  return mTag;
}

auto Object::get_persistent_id() const -> Maybe<int32>
{
  return mPersistentId;
}

auto Object::is_visible() const -> bool
{
  return mVisible;
}

auto Object::get_meta() -> Metadata&
{
  return mMeta;
}

auto Object::get_meta() const -> const Metadata&
{
  return mMeta;
}

auto Object::clone() const -> Object
{
  Object other {mType};

  // The persistent ID attribute is intentionally ignored.
  other.mMeta = mMeta.clone();
  other.mPosition = mPosition;
  other.mSize = mSize;
  other.mType = mType;
  other.mVisible = mVisible;

  return other;
}

}  // namespace tactile
