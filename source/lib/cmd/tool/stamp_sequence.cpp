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

#include "common/debug/assert.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"
#include "model/layers/layer_components.hpp"
#include "model/layers/tile_layer_ops.hpp"

namespace tactile::cmd {

StampSequence::StampSequence(Registry* registry,
                             const Entity tile_layer_entity,
                             TileCache old_state,
                             TileCache new_state)
    : mRegistry {registry},
      mTileLayerEntity {tile_layer_entity},
      mOldState {std::move(old_state)},
      mNewState {std::move(new_state)}
{
  TACTILE_ASSERT(sys::is_tile_layer_entity(*mRegistry, mTileLayerEntity));
}

void StampSequence::undo()
{
  _apply_sequence(mOldState);
}

void StampSequence::redo()
{
  _apply_sequence(mNewState);
}

auto StampSequence::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mRegistry);
  return strings.cmd.stamp_tool;
}

void StampSequence::_apply_sequence(const TileCache& cache)
{
  auto& registry = *mRegistry;
  auto& tile_layer = registry.get<TileLayer>(mTileLayerEntity);

  for (const auto& [position, tile_id]: cache) {
    sys::set_tile(tile_layer, position, tile_id);
  }
}

}  // namespace tactile::cmd
