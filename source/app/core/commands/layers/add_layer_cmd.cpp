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

#include "add_layer_cmd.hpp"

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/documents/map_document.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddLayerCmd::AddLayerCmd(MapDocument* map, const LayerType type)
    : ACommand{"Add Layer"}
    , mMap{map}
    , mLayerType{type}
{
  if (!mMap) {
    throw TactileError{"Invalid null map!"};
  }
}

void AddLayerCmd::undo()
{
  auto& registry = mMap->get_registry();

  const auto layerEntity = sys::find_context(registry, mLayerId.value());
  TACTILE_ASSERT(layerEntity != entt::null);

  mLayerSnapshot = sys::remove_layer(registry, layerEntity);
}

void AddLayerCmd::redo()
{
  auto& registry = mMap->get_registry();

  if (mLayerSnapshot) {
    sys::restore_layer(registry, *mLayerSnapshot);
  }
  else {
    entt::entity entity{entt::null};
    switch (mLayerType) {
      case LayerType::TileLayer: {
        entity = sys::new_tile_layer(registry);
        break;
      }
      case LayerType::ObjectLayer: {
        entity = sys::new_object_layer(registry);
        break;
      }
      case LayerType::GroupLayer: {
        entity = sys::new_group_layer(registry);
        break;
      }
    }

    TACTILE_ASSERT(entity != entt::null);
    mLayerId = checked_get<comp::Context>(registry, entity).id;

    sys::sort_layers(registry);
  }
}

}  // namespace tactile
