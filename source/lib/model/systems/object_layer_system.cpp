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

#include "object_layer_system.hpp"

#include "model/components/layer.hpp"
#include "model/components/object.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _get_bounds(const Object& object, const Float2& tile_size) -> Float4
{
  if (object.type == ObjectType::Point) {
    return {object.position - (tile_size * 0.25f), tile_size * 0.5f};
  }
  else {
    return {object.position, object.size};
  }
}

}  // namespace

auto find_object_at_position(const Model& model,
                             const Entity object_layer_entity,
                             const Float2& target_pos,
                             const Float2& tile_size) -> Entity
{
  const auto& object_layer = model.get<ObjectLayer>(object_layer_entity);

  for (const auto object_entity: object_layer.objects) {
    const auto& object = model.get<Object>(object_entity);

    const auto object_bounds = _get_bounds(object, tile_size);
    const auto max_object_x = object_bounds.x + object_bounds.z;
    const auto max_object_y = object_bounds.y + object_bounds.w;

    if (target_pos.x >= object_bounds.x &&  //
        target_pos.y >= object_bounds.y &&  //
        target_pos.x <= max_object_x &&     //
        target_pos.y <= max_object_y) {
      return object_entity;
    }
  }

  return kNullEntity;
}

}  // namespace tactile::sys
