// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "object_layer.hpp"

#include <utility>  // move

#include "tactile/core/debug/exception.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto get_bounds(const Object& object, const Float2& tile_size) -> Float4
{
  if (object.is_point()) {
    return {object.get_pos() - (tile_size * 0.25f), tile_size * 0.5f};
  }
  else {
    return {object.get_pos(), object.get_size()};
  }
}

}  // namespace

void ObjectLayer::accept(ContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void ObjectLayer::accept(LayerVisitor& visitor)
{
  visitor.visit(*this);
}

void ObjectLayer::accept(ConstLayerVisitor& visitor) const
{
  visitor.visit(*this);
}

void ObjectLayer::add_object(Shared<Object> object)
{
  const auto id = object->get_uuid();
  mObjects[id] = std::move(object);
}

void ObjectLayer::remove_object(const UUID& id)
{
  mObjects.erase(id);

  if (mActiveObject == id) {
    mActiveObject.reset();
  }
}

void ObjectLayer::reserve_objects(const usize n)
{
  mObjects.reserve(n);
}

void ObjectLayer::select_object(const Maybe<UUID>& id)
{
  mActiveObject = id;
  if (mActiveObject && !has_object(*mActiveObject)) {
    throw Exception {"Invalid object identifier!"};
  }
}

auto ObjectLayer::has_object(const UUID& id) const -> bool
{
  return mObjects.find(id) != mObjects.end();
}

auto ObjectLayer::object_count() const -> usize
{
  return mObjects.size();
}

auto ObjectLayer::active_object_id() const -> Maybe<UUID>
{
  return mActiveObject;
}

auto ObjectLayer::get_object_ptr(const UUID& id) -> const Shared<Object>&
{
  return lookup_in(mObjects, id);
}

auto ObjectLayer::get_object(const UUID& id) -> Object&
{
  return *lookup_in(mObjects, id);
}

auto ObjectLayer::get_object(const UUID& id) const -> const Object&
{
  return *lookup_in(mObjects, id);
}

auto ObjectLayer::object_at(const Float2& pos, const Float2& tile_size) const
    -> Maybe<UUID>
{
  for (const auto& [id, object] : mObjects) {
    const auto bounds = get_bounds(*object, tile_size);

    const auto max_x = bounds.x() + bounds.z();
    const auto max_y = bounds.y() + bounds.w();

    if (pos.x() >= bounds.x() && pos.y() >= bounds.y() && pos.x() <= max_x &&
        pos.y() <= max_y) {
      return id;
    }
  }

  return nothing;
}

auto ObjectLayer::clone() const -> Shared<Layer>
{
  auto copy = std::make_shared<ObjectLayer>();
  copy->mDelegate = mDelegate.clone();

  // FIXME clone objects

  return copy;
}

}  // namespace tactile
