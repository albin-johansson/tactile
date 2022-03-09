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

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

AddLayerCmd::AddLayerCmd(registry_ref registry, const layer_type type)
    : ACommand{"Add Layer"}
    , mRegistry{registry}
    , mLayerType{type}
{}

void AddLayerCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto id = mLayerId.value();
  const auto entity = sys::find_layer(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = sys::remove_layer(registry, entity);
}

void AddLayerCmd::redo()
{
  if (mLayerSnapshot) {
    sys::restore_layer(mRegistry, *mLayerSnapshot);
  }
  else {
    auto& registry = mRegistry.get();

    entt::entity entity{entt::null};
    switch (mLayerType) {
      case layer_type::tile_layer: {
        entity = sys::make_tile_layer(registry);
        break;
      }
      case layer_type::object_layer: {
        entity = sys::make_object_layer(registry);
        break;
      }
      case layer_type::group_layer: {
        entity = sys::make_group_layer(registry);
        break;
      }
    }

    TACTILE_ASSERT(entity != entt::null);
    mLayerId = registry.get<comp::Layer>(entity).id;

    sys::sort_layers(registry);
  }
}

}  // namespace tactile
