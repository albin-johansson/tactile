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

#include "layer_system.hpp"

#include "common/debug/assert.hpp"
#include "common/type/string.hpp"
#include "model/components/layer.hpp"
#include "model/systems/validation_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _get_parent_layer(const Model& model,
                                     const Entity root_layer_entity,
                                     const Entity target_layer_entity) -> Entity
{
  TACTILE_ASSERT(is_group_layer_entity(model, root_layer_entity));
  TACTILE_ASSERT(is_layer_entity(model, target_layer_entity));

  const auto& root_layer = model.get<GroupLayer>(root_layer_entity);

  for (const auto layer_entity: root_layer.children) {
    if (layer_entity == target_layer_entity) {
      return root_layer_entity;
    }
    else if (model.has<GroupLayer>(layer_entity)) {
      const auto parent_entity =
          _get_parent_layer(model, layer_entity, target_layer_entity);

      if (parent_entity != kNullEntity) {
        return parent_entity;
      }
    }
  }

  return kNullEntity;
}

}  // namespace

auto get_parent_layer(const Model& model, const Map& map, const Entity layer_entity)
    -> Entity
{
  return _get_parent_layer(model, map.root_layer, layer_entity);
}

}  // namespace tactile::sys
