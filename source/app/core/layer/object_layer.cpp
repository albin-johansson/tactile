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

#include "object_layer.hpp"

#include <utility>  // move

#include "core/util/assoc.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto get_bounds(const Object& object, const Float2& tile_size) -> Float4
{
  if (object.is_point()) {
    return {object.pos() - (tile_size * 0.25f), tile_size * 0.5f};
  }
  else {
    return {object.pos(), object.size()};
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

void ObjectLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void ObjectLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

void ObjectLayer::set_parent(const Maybe<UUID>& parent_id)
{
  mDelegate.set_parent(parent_id);
}

void ObjectLayer::set_meta_id(const int32 id)
{
  mDelegate.set_meta_id(id);
}

void ObjectLayer::add_object(Shared<Object> object)
{
  const auto id = object->uuid();
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
    throw TactileError {"Invalid object identifier!"};
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
  for (const auto& [id, object]: mObjects) {
    const auto bounds = get_bounds(*object, tile_size);

    const auto max_x = bounds.x + bounds.z;
    const auto max_y = bounds.y + bounds.w;

    if (pos.x >= bounds.x && pos.y >= bounds.y && pos.x <= max_x && pos.y <= max_y) {
      return id;
    }
  }

  return nothing;
}

auto ObjectLayer::opacity() const -> float
{
  return mDelegate.get_opacity();
}

auto ObjectLayer::visible() const -> bool
{
  return mDelegate.is_visible();
}

auto ObjectLayer::clone() const -> Shared<Layer>
{
  auto copy = std::make_shared<ObjectLayer>();
  copy->mDelegate = mDelegate.clone();

  // FIXME clone objects

  return copy;
}

auto ObjectLayer::ctx() -> ContextInfo&
{
  return mDelegate.ctx();
}

auto ObjectLayer::ctx() const -> const ContextInfo&
{
  return mDelegate.ctx();
}

auto ObjectLayer::uuid() const -> const UUID&
{
  return mDelegate.ctx().uuid();
}

auto ObjectLayer::parent() const -> Maybe<UUID>
{
  return mDelegate.get_parent();
}

auto ObjectLayer::meta_id() const -> Maybe<int32>
{
  return mDelegate.get_meta_id();
}

auto ObjectLayer::type() const -> LayerType
{
  return LayerType::ObjectLayer;
}

}  // namespace tactile
