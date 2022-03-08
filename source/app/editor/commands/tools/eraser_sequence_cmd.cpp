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

#include "eraser_sequence_cmd.hpp"

#include <utility>  // move

#include "core/components/layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

eraser_sequence_cmd::eraser_sequence_cmd(registry_ref registry, TileCache&& oldState)
    : command_base{"Eraser Sequence"}
    , mRegistry{registry}
    , mLayer{sys::get_active_layer_id(registry).value()}
    , mOldState{std::move(oldState)}
{}

void eraser_sequence_cmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::get_tile_layer_entity(registry, mLayer);
  auto& layer = registry.get<comp::tile_layer>(entity);

  sys::set_tiles(layer, mOldState);
}

void eraser_sequence_cmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayer);
  TACTILE_ASSERT(entity != entt::null);

  auto& matrix = registry.get<comp::tile_layer>(entity).matrix;
  for (const auto& [position, _] : mOldState) {
    TACTILE_ASSERT(position.row_index() < matrix.size());
    TACTILE_ASSERT(position.col_index() < matrix.front().size());
    matrix[position.row_index()][position.col_index()] = empty_tile;
  }
}

}  // namespace tactile
