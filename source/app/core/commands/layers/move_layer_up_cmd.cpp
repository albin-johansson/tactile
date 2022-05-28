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

#include "move_layer_up_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"

namespace tactile {

MoveLayerUpCmd::MoveLayerUpCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Move Layer Up"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerUpCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto layerEntity = sys::get_layer(registry, mLayerId);
  sys::move_layer_down(registry, layerEntity);
}

void MoveLayerUpCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto layerEntity = sys::get_layer(registry, mLayerId);
  sys::move_layer_up(registry, layerEntity);
}

}  // namespace tactile