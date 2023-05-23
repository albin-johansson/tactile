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

#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "components/map.hpp"
#include "core/layer.hpp"
#include "core/object.hpp"
#include "model/context.hpp"
#include "systems/language_system.hpp"

namespace tactile::cmd {

RemoveObject::RemoveObject(const ObjectLayerEntity layer_entity,
                           const ObjectEntity object_entity)
    : mLayerEntity {layer_entity},
      mObjectEntity {object_entity}
{
}

void RemoveObject::undo()
{
  auto& model = get_global_model();

  auto& object_layer = model.get<ObjectLayer>(mLayerEntity);
  object_layer.objects.push_back(mObjectEntity);

  model.set_enabled(mObjectEntity, true);
  mRemovedObject = false;
}

void RemoveObject::redo()
{
  auto& model = get_global_model();
  auto& object_layer = model.get<ObjectLayer>(mLayerEntity);

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
    spdlog::trace("[CMD] Disposing removed object");

    auto& model = get_global_model();
    model.destroy(mObjectEntity);
  }
}

auto RemoveObject::get_name() const -> String
{
  const auto& model = get_global_model();

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
