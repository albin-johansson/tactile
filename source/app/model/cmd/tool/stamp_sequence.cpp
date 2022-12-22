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

#include "stamp_sequence.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

StampSequence::StampSequence(Shared<Map> map,
                             const UUID& layer_id,
                             TileCache old_state,
                             TileCache new_state)
    : mMap {std::move(map)},
      mLayerId {layer_id},
      mOldState {std::move(old_state)},
      mNewState {std::move(new_state)}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
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
  const auto& lang = get_current_language();
  return lang.cmd.stamp_tool;
}

void StampSequence::apply_sequence(const TileCache& cache)
{
  auto& layer = mMap->invisible_root().tile_layer(mLayerId);
  for (const auto& [pos, tile]: cache) {
    layer.set_tile(pos, tile);
  }
}

}  // namespace tactile::cmd
