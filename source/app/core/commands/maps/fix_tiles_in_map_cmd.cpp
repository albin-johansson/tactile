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

#include "fix_tiles_in_map_cmd.hpp"

#include <utility>  // move

#include "core/layers/tile_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

FixTilesInMapCmd::FixTilesInMapCmd(Shared<Map> map) : mMap{std::move(map)}
{
  if (!mMap) {
    throw TactileError{"Invalid null map!"};
  }
}

void FixTilesInMapCmd::undo()
{
  for (const auto& [layerId, previous] : mResult) {
    auto& layer = mMap->view_tile_layer(layerId);

    for (const auto& [pos, tile] : previous) {
      layer.set_tile(pos, tile);
    }
  }

  mResult.clear();
}

void FixTilesInMapCmd::redo()
{
  mResult = mMap->fix_tiles();
}

auto FixTilesInMapCmd::get_name() const -> const char*
{
  return "Fix Tiles In Map";
}

}  // namespace tactile
