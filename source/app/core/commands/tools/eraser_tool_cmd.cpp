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

#include "eraser_tool_cmd.hpp"

#include <utility>  // move

#include "core/common/ecs.hpp"
#include "core/components/layers.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

EraserToolCmd::EraserToolCmd(RegistryRef registry, TileCache&& oldState)
    : ACommand{"Eraser Sequence"}
    , mRegistry{registry}
    , mLayerId{sys::get_active_layer_id(registry).value()}
    , mOldState{std::move(oldState)}
{}

void EraserToolCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto layerEntity = sys::find_context(registry, mLayerId);
  auto& layer = checked_get<comp::TileLayer>(registry, layerEntity);

  sys::set_tiles(layer, mOldState);
}

void EraserToolCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_context(registry, mLayerId);
  auto& matrix = checked_get<comp::TileLayer>(registry, entity).matrix;
  for (const auto& [position, _] : mOldState) {
    TACTILE_ASSERT(position.row_index() < matrix.size());
    TACTILE_ASSERT(position.col_index() < matrix.front().size());
    matrix[position.row_index()][position.col_index()] = empty_tile;
  }
}

}  // namespace tactile
