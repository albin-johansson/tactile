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

#include "core/documents/map_document.hpp"
#include "core/layers/tile_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

FixTilesInMapCmd::FixTilesInMapCmd(MapDocument* document)
    : ACommand{"Fix Tiles In Map"}
    , mDocument{document}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void FixTilesInMapCmd::undo()
{
  auto& map = mDocument->get_map();

  for (const auto& [layerId, previous] : mResult) {
    auto& layer = map.view_tile_layer(layerId);

    for (const auto& [pos, tile] : previous) {
      layer.set_tile(pos, tile);
    }
  }

  mResult.clear();
}

void FixTilesInMapCmd::redo()
{
  auto& map = mDocument->get_map();
  mResult = map.fix_tiles();
}

}  // namespace tactile
