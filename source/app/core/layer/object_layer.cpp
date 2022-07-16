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

#include "core/layer/layer_visitor.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto _get_bounds(const Object& object, const Vector2f& tileSize) -> Vector4f
{
  if (object.get_type() == ObjectType::Point) {
    return {object.get_pos() - (tileSize * 0.25f), tileSize * 0.5f};
  }
  else {
    return {object.get_pos(), object.get_size()};
  }
}

}  // namespace

auto ObjectLayer::make() -> Shared<ObjectLayer>
{
  return std::make_shared<ObjectLayer>();
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

void ObjectLayer::set_parent(const Maybe<UUID>& parentId)
{
  mDelegate.set_parent(parentId);
}

void ObjectLayer::set_meta_id(const int32 id)
{
  mDelegate.set_meta_id(id);
}

void ObjectLayer::accept(IContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void ObjectLayer::set_name(std::string name)
{
  mDelegate.set_name(std::move(name));
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

auto ObjectLayer::object_at(const Vector2f& pos, const Vector2f& tileSize) const
    -> Maybe<UUID>
{
  for (const auto& [id, object] : mObjects) {
    const auto bounds = _get_bounds(*object, tileSize);

    const auto maxX = bounds.x + bounds.z;
    const auto maxY = bounds.y + bounds.w;

    if (pos.x >= bounds.x && pos.y >= bounds.y && pos.x <= maxX && pos.y <= maxY) {
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
  auto copy = make();
  copy->mDelegate = mDelegate.clone();
  return copy;
}

auto ObjectLayer::get_name() const -> const std::string&
{
  return mDelegate.get_name();
}

auto ObjectLayer::get_props() -> PropertyBundle&
{
  return mDelegate.get_props();
}

auto ObjectLayer::get_props() const -> const PropertyBundle&
{
  return mDelegate.get_props();
}

auto ObjectLayer::get_comps() -> ComponentBundle&
{
  return mDelegate.get_comps();
}

auto ObjectLayer::get_comps() const -> const ComponentBundle&
{
  return mDelegate.get_comps();
}

auto ObjectLayer::get_uuid() const -> const UUID&
{
  return mDelegate.get_uuid();
}

auto ObjectLayer::get_parent() const -> Maybe<UUID>
{
  return mDelegate.get_parent();
}

auto ObjectLayer::get_meta_id() const -> Maybe<int32>
{
  return mDelegate.get_meta_id();
}

}  // namespace tactile
