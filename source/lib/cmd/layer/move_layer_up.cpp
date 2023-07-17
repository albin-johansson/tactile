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

#include "move_layer_up.hpp"

#include "common/debug/assert.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/maps/map_components.hpp"

namespace tactile::cmd {

MoveLayerUp::MoveLayerUp(Registry* registry,
                         const Entity map_entity,
                         const Entity layer_entity)
    : mRegistry {registry},
      mMapEntity {map_entity},
      mLayerEntity {layer_entity}
{
  TACTILE_ASSERT(sys::is_map_entity(*mRegistry, mMapEntity));
  TACTILE_ASSERT(sys::is_layer_entity(*mRegistry, mLayerEntity));
}

void MoveLayerUp::undo()
{
  auto& registry = *mRegistry;

  const auto& map = registry.get<Map>(mMapEntity);
  sys::move_layer_down(registry, map, mLayerEntity);
}

void MoveLayerUp::redo()
{
  auto& registry = *mRegistry;

  const auto& map = registry.get<Map>(mMapEntity);
  sys::move_layer_up(registry, map, mLayerEntity);
}

auto MoveLayerUp::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mRegistry);
  return strings.cmd.move_layer_up;
}

}  // namespace tactile::cmd
