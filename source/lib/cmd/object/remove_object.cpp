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

#include "model/entity_validation.hpp"
#include "model/layers/layer_components.hpp"
#include "model/maps/map_components.hpp"
#include "model/objects/object_components.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "tactile/core/common/assert.hpp"
#include "tactile/core/common/error.hpp"

namespace tactile::cmd {

RemoveObject::RemoveObject(Registry* registry,
                           const Entity object_layer_entity,
                           const Entity object_entity)
    : mRegistry {registry},
      mObjectLayerEntity {object_layer_entity},
      mObjectEntity {object_entity}
{
  TACTILE_ASSERT(sys::is_object_layer_entity(*mRegistry, mObjectLayerEntity));
  TACTILE_ASSERT(sys::is_object_entity(*mRegistry, mObjectEntity));
}

void RemoveObject::undo()
{
  auto& registry = *mRegistry;

  auto& object_layer = registry.get<ObjectLayer>(mObjectLayerEntity);
  object_layer.objects.push_back(mObjectEntity);

  registry.set_enabled(mObjectEntity, true);
  mRemovedObject = false;
}

void RemoveObject::redo()
{
  auto& registry = *mRegistry;
  auto& object_layer = registry.get<ObjectLayer>(mObjectLayerEntity);

  if (object_layer.active_object == mObjectEntity) {
    object_layer.active_object = kNullEntity;
  }

  std::erase(object_layer.objects, mObjectEntity);

  registry.set_enabled(mObjectEntity, false);
  mRemovedObject = true;
}

void RemoveObject::dispose()
{
  if (mRemovedObject) {
    mRegistry->destroy(mObjectEntity);
  }
}

auto RemoveObject::get_name() const -> String
{
  const auto& language_service = ServiceLocator<LanguageService>::get();
  const auto& strings = language_service.get_current_language_strings();

  const auto& registry = *mRegistry;
  const auto& object = registry.get<Object>(mObjectEntity);

  switch (object.type) {
    case ObjectType::Point:
      return strings.cmd.remove_point_object;

    case ObjectType::Rect:
      return strings.cmd.remove_rectangle_object;

    case ObjectType::Ellipse:
      return strings.cmd.remove_ellipse_object;

    default:
      throw Error {"Invalid object type"};
  }
}

}  // namespace tactile::cmd
