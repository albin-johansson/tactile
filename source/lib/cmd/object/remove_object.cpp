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

#include "remove_object.hpp"

#include "common/debug/assert.hpp"
#include "common/debug/panic.hpp"
#include "components/layer.hpp"
#include "components/map.hpp"
#include "components/object.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::cmd {

RemoveObject::RemoveObject(Model* model,
                           const Entity object_layer_entity,
                           const Entity object_entity)
    : mModel {model},
      mObjectLayerEntity {object_layer_entity},
      mObjectEntity {object_entity}
{
  TACTILE_ASSERT(sys::is_object_layer_entity(*mModel, mObjectLayerEntity));
  TACTILE_ASSERT(sys::is_object_entity(*mModel, mObjectEntity));
}

void RemoveObject::undo()
{
  auto& model = *mModel;

  auto& object_layer = model.get<ObjectLayer>(mObjectLayerEntity);
  object_layer.objects.push_back(mObjectEntity);

  model.set_enabled(mObjectEntity, true);
  mRemovedObject = false;
}

void RemoveObject::redo()
{
  auto& model = *mModel;
  auto& object_layer = model.get<ObjectLayer>(mObjectLayerEntity);

  if (object_layer.active_object == mObjectEntity) {
    object_layer.active_object = kNullEntity;
  }

  std::erase(object_layer.objects, mObjectEntity);

  model.set_enabled(mObjectEntity, false);
  mRemovedObject = true;
}

void RemoveObject::dispose()
{
  if (mRemovedObject) {
    mModel->destroy(mObjectEntity);
  }
}

auto RemoveObject::get_name() const -> String
{
  const auto& model = *mModel;

  const auto& strings = sys::get_current_language_strings(model);
  const auto& object = model.get<Object>(mObjectEntity);

  switch (object.type) {
    case ObjectType::Point:
      return strings.cmd.remove_point_object;

    case ObjectType::Rect:
      return strings.cmd.remove_rectangle_object;

    case ObjectType::Ellipse:
      return strings.cmd.remove_ellipse_object;

    default:
      throw TactileError {"Invalid object type"};
  }
}

}  // namespace tactile::cmd
