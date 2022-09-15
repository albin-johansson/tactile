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

#include "misc/panic.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto get_bounds(const Object& object, const float2& tile_size) -> Vector4f
{
  if (object.is_point()) {
    return {object.get_pos() - (tile_size * 0.25f), tile_size * 0.5f};
  }
  else {
    return {object.get_pos(), object.get_size()};
  }
}

}  // namespace

void ObjectLayer::accept(IContextVisitor& visitor) const
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
  const auto id = object->get_uuid();
  mObjects[id] = std::move(object);
}

void ObjectLayer::remove_object(const UUID& id)
{
  if (const auto iter = mObjects.find(id); iter != mObjects.end()) {
    mObjects.erase(iter);
  }
  else {
    throw TactileError {"Invalid object identifier!"};
  }
}

void ObjectLayer::reserve_objects(const usize n)
{
  mObjects.reserve(n);
}

void ObjectLayer::select_object(const Maybe<UUID>& id)
{
  mActiveObject = id;
  if (mActiveObject && !mObjects.contains(*mActiveObject)) {
    throw TactileError {"Invalid object identifier!"};
  }
}

auto ObjectLayer::has_object(const UUID& id) const -> bool
{
  return mObjects.contains(id);
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

auto ObjectLayer::object_at(const float2& pos, const float2& tile_size) const
    -> Maybe<UUID>
{
  for (const auto& [id, object] : mObjects) {
    const auto bounds = get_bounds(*object, tile_size);

    const auto max_x = bounds.x + bounds.z;
    const auto max_y = bounds.y + bounds.w;

    if (pos.x >= bounds.x && pos.y >= bounds.y && pos.x <= max_x && pos.y <= max_y) {
      return id;
    }
  }

  return nothing;
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

auto ObjectLayer::get_uuid() const -> const UUID&
{
  return mDelegate.ctx().uuid();
}

auto ObjectLayer::get_parent() const -> Maybe<UUID>
{
  return mDelegate.get_parent();
}

auto ObjectLayer::get_meta_id() const -> Maybe<int32>
{
  return mDelegate.get_meta_id();
}

auto ObjectLayer::get_type() const -> LayerType
{
  return LayerType::ObjectLayer;
}

}  // namespace tactile
