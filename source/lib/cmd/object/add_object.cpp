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

#include "common/debug/assert.hpp"
#include "common/debug/panic.hpp"
#include "components/layer.hpp"
#include "components/map.hpp"
#include "components/object.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/object_system.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::cmd {

AddObject::AddObject(Model* model,
                     const Entity map_entity,
                     const Entity object_layer_entity,
                     const ObjectType type,
                     const Float2 position,
                     const Float2 size)
    : mModel {model},
      mMapEntity {map_entity},
      mObjectLayerEntity {object_layer_entity},
      mObjectType {type},
      mPosition {position},
      mSize {size}
{
  TACTILE_ASSERT(sys::is_map_entity(*mModel, mMapEntity));
  TACTILE_ASSERT(sys::is_object_layer_entity(*mModel, mObjectLayerEntity));
}

void AddObject::undo()
{
  auto& model = *mModel;
  auto& object_layer = model.get<ObjectLayer>(mObjectLayerEntity);

  const auto object_entity = mObjectEntity.value();
  std::erase(object_layer.objects, object_entity);

  model.set_enabled(object_entity, false);
  mDidAddObject = false;
}

void AddObject::redo()
{
  auto& model = *mModel;
  auto& object_layer = model.get<ObjectLayer>(mObjectLayerEntity);

  if (!mObjectEntity.has_value()) {
    mObjectEntity = sys::create_object(model, mObjectType);
  }

  const auto object_entity = mObjectEntity.value();
  model.set_enabled(object_entity, true);

  auto& object = model.get<Object>(object_entity);
  object.type = mObjectType;
  object.position = mPosition;
  object.size = mSize;

  if (!object.meta_id.has_value()) {
    auto& map_identifiers = model.get<MapIdentifiers>(mMapEntity);
    object.meta_id = map_identifiers.next_object_id++;
  }

  object_layer.objects.push_back(object_entity);
  mDidAddObject = true;
}

void AddObject::dispose()
{
  if (mObjectEntity.has_value() && !mDidAddObject) {
    mModel->destroy(*mObjectEntity);
  }
}

auto AddObject::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);

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
