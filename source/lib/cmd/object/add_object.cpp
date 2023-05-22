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

#include "add_object.hpp"

#include "common/debug/panic.hpp"
#include "core/layer.hpp"
#include "core/map.hpp"
#include "model/context.hpp"
#include "model/systems/object_system.hpp"
#include "systems/language_system.hpp"

namespace tactile::cmd {

AddObject::AddObject(const MapEntity map_entity,
                     const ObjectLayerEntity layer_entity,
                     const ObjectType type,
                     const Float2 position,
                     const Float2 size)
    : mMapEntity {map_entity},
      mLayerEntity {layer_entity},
      mObjectType {type},
      mPosition {position},
      mSize {size}
{
}

void AddObject::undo()
{
  auto& model = get_global_model();
  auto& object_layer = model.get<ObjectLayer>(mLayerEntity);

  const auto object_entity = mObjectEntity.value();
  std::erase(object_layer.objects, object_entity);

  model.set_enabled(object_entity, false);
  mDidAddObject = false;
}

void AddObject::redo()
{
  auto& model = get_global_model();

  auto& map = model.get<Map>(mMapEntity);
  auto& object_layer = model.get<ObjectLayer>(mLayerEntity);

  if (!mObjectEntity.has_value()) {
    mObjectEntity = sys::create_object(model, mObjectType);
  }

  const auto object_entity = mObjectEntity.value();
  model.set_enabled(object_entity, true);

  auto& object = model.get<Object>(object_entity);
  object.type = mObjectType;
  object.position = mPosition;
  object.size = mSize;
  object.meta_id = map.next_object_id++;

  object_layer.objects.push_back(object_entity);
  mDidAddObject = true;
}

void AddObject::dispose()
{
  if (mObjectEntity.has_value() && !mDidAddObject) {
    auto& model = get_global_model();
    model.destroy(*mObjectEntity);
  }
}

auto AddObject::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  switch (mObjectType) {
    case ObjectType::Point:
      return strings.cmd.add_point_object;

    case ObjectType::Rect:
      return strings.cmd.add_rectangle_object;

    case ObjectType::Ellipse:
      return strings.cmd.add_ellipse_object;

    default:
      throw TactileError {"Invalid object type"};
  }
}

}  // namespace tactile::cmd
