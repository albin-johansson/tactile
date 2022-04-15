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

#include "move_layer_down_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"

namespace tactile {

MoveLayerDownCmd::MoveLayerDownCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Move Layer Down"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerDownCmd::undo()
{
  auto& registry = mRegistry.get();
  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_up(registry, entity);
}

void MoveLayerDownCmd::redo()
{
  auto& registry = mRegistry.get();
  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_down(registry, entity);
}

}  // namespace tactile
