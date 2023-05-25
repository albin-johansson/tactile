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

#include "stamp_sequence.hpp"

#include <utility>  // move

#include "core/layer.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

StampSequence::StampSequence(const Entity tile_layer_entity,
                             TileCache old_state,
                             TileCache new_state)
    : mTileLayerEntity {tile_layer_entity},
      mOldState {std::move(old_state)},
      mNewState {std::move(new_state)}
{
}

void StampSequence::undo()
{
  apply_sequence(mOldState);
}

void StampSequence::redo()
{
  apply_sequence(mNewState);
}

auto StampSequence::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.stamp_tool;
}

void StampSequence::apply_sequence(const TileCache& cache)
{
  auto& model = get_global_model();
  auto& tile_layer = model.get<TileLayer>(mTileLayerEntity);

  for (const auto& [pos, tile]: cache) {
    tile_layer.set_tile(pos, tile);
  }
}

}  // namespace tactile::cmd
