// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/object_layer.hpp"

#include <utility>  // move

#include "tactile/core/layer/layer_visitor.hpp"
#include "tactile/foundation/container/lookup.hpp"
#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

void ObjectLayer::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);

  for (auto& [object_id, object] : mObjects) {
    object.accept(visitor);
  }
}

void ObjectLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
}

void ObjectLayer::accept(IConstLayerVisitor& visitor) const
{
  visitor.visit(*this);
}

auto ObjectLayer::add_object(Object object) -> Result<void>
{
  if (const auto object_id = object.get_persistent_id()) {
    for (const auto& [_, other_object] : mObjects) {
      if (object_id == other_object.get_persistent_id()) {
        TACTILE_LOG_WARN("Detected duplicated persistent object identifier '{}'",
                         *object_id);
        return unexpected(make_generic_error(GenericError::kInvalidState));
      }
    }
  }

  const auto object_uuid = object.meta().uuid();
  mObjects.insert_or_assign(object_uuid, std::move(object));

  return kOK;
}

auto ObjectLayer::emplace_object(const ObjectType type) -> Object&
{
  Object object {type};
  auto [iter, _] = mObjects.insert_or_assign(object.meta().uuid(), std::move(object));
  return iter->second;
}

auto ObjectLayer::remove_object(const UUID& uuid) -> Maybe<Object>
{
  return erase_from(mObjects, uuid);
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

auto ObjectLayer::find_object(const UUID& uuid) -> Object*
{
  const auto iter = mObjects.find(uuid);
  return (iter != mObjects.end()) ? &iter->second : nullptr;
}

auto ObjectLayer::find_object(const UUID& uuid) const -> const Object*
{
  const auto iter = mObjects.find(uuid);
  return (iter != mObjects.end()) ? &iter->second : nullptr;
}

auto ObjectLayer::get_object(const UUID& uuid) -> Object&
{
  return *require_not_null(find_object(uuid), "invalid object UUID");
}

auto ObjectLayer::get_object(const UUID& uuid) const -> const Object&
{
  return *require_not_null(find_object(uuid), "invalid object UUID");
}

auto ObjectLayer::has_object(const UUID& uuid) const -> bool
{
  return mObjects.contains(uuid);
}

auto ObjectLayer::object_count() const -> ssize
{
  return std::ssize(mObjects);
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
    other->mObjects.insert_or_assign(object_id, object.clone());
  }

  return other;
}

auto ObjectLayer::meta() -> Metadata&
{
  return mDelegate.meta();
}

auto ObjectLayer::meta() const -> const Metadata&
{
  return mDelegate.meta();
}

}  // namespace tactile::core
