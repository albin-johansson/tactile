// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/layer/object_layer.hpp"

#include <utility>  // move

#include "tactile/core/map/layer/layer_visitor.hpp"
#include "tactile/foundation/container/lookup.hpp"

namespace tactile {

void ObjectLayer::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);
}

void ObjectLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
}

void ObjectLayer::accept(IConstLayerVisitor& visitor) const
{
  visitor.visit(*this);
}

void ObjectLayer::add_object(const ObjectID id, Shared<Object> object)
{
  mObjects[id] = std::move(object);
}

auto ObjectLayer::remove_object(const ObjectID id) -> Shared<Object>
{
  if (auto object = erase_from(mObjects, id)) {
    return std::move(object).value();
  }

  return nullptr;
}

void ObjectLayer::set_persistent_id(const Maybe<int32> id)
{
  mDelegate.set_persistent_id(id);
}

void ObjectLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void ObjectLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

auto ObjectLayer::get_object(const ObjectID id) -> Shared<Object>
{
  return lookup_in(mObjects, id);
}

auto ObjectLayer::find_object(const ObjectID id) -> Object*
{
  const auto iter = mObjects.find(id);
  return (iter != mObjects.end()) ? iter->second.get() : nullptr;
}

auto ObjectLayer::find_object(const ObjectID id) const -> const Object*
{
  const auto iter = mObjects.find(id);
  return (iter != mObjects.end()) ? iter->second.get() : nullptr;
}

auto ObjectLayer::has_object(const ObjectID id) const -> bool
{
  return mObjects.contains(id);
}

auto ObjectLayer::object_count() const -> usize
{
  return mObjects.size();
}

auto ObjectLayer::get_persistent_id() const -> Maybe<int32>
{
  return mDelegate.get_persistent_id();
}

auto ObjectLayer::get_opacity() const -> float
{
  return mDelegate.get_opacity();
}

auto ObjectLayer::is_visible() const -> bool
{
  return mDelegate.is_visible();
}

auto ObjectLayer::clone() const -> Shared<ILayer>
{
  auto other = make_shared<ObjectLayer>();

  // The persistent ID attribute is intentionally ignored.
  other->mDelegate = mDelegate.clone();

  for (const auto& [object_id, object] : mObjects) {
    other->mObjects[object_id] = object->clone();
  }

  return other;
}

auto ObjectLayer::get_meta() -> Metadata&
{
  return mDelegate.get_meta();
}

auto ObjectLayer::get_meta() const -> const Metadata&
{
  return mDelegate.get_meta();
}

}  // namespace tactile
