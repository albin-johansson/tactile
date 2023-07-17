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

#include "layer_recursion.hpp"

namespace tactile::sys {

void visit_layers(const Registry& registry,
                  const GroupLayer& root_layer,
                  const EntityCallback& callback)
{
  for (const auto child_layer_entity: root_layer.children) {
    if (const auto* child_group_layer =
            registry.try_get<GroupLayer>(child_layer_entity)) {
      visit_layers(registry, *child_group_layer, callback);
    }

    callback(child_layer_entity);
  }
}

void visit_layers(const Registry& registry,
                  const Map& map,
                  const EntityCallback& callback)
{
  const auto& root_layer = registry.get<GroupLayer>(map.root_layer);
  visit_layers(registry, root_layer, callback);
}

void visit_tile_layers(const Registry& registry,
                       const GroupLayer& root_layer,
                       const EntityCallback& callback)
{
  visit_layers(registry, root_layer, [&](const Entity layer_entity) {
    if (registry.has<TileLayer>(layer_entity)) {
      callback(layer_entity);
    }
  });
}

void visit_tile_layers(const Registry& registry,
                       const Map& map,
                       const EntityCallback& callback)
{
  const auto& root_layer = registry.get<GroupLayer>(map.root_layer);
  visit_tile_layers(registry, root_layer, callback);
}

}  // namespace tactile::sys
