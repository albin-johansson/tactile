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

#include "stamp_tool_cmd.hpp"

#include <utility>  // move

#include "core/documents/map_document.hpp"
#include "core/layers/tile_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

StampToolCmd::StampToolCmd(MapDocument* document,
                           const UUID&  layerId,
                           TileCache    oldState,
                           TileCache    newState)
    : mDocument{document}
    , mLayerId{layerId}
    , mOldState{std::move(oldState)}
    , mNewState{std::move(newState)}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void StampToolCmd::undo()
{
  apply_sequence(mOldState);
}

void StampToolCmd::redo()
{
  apply_sequence(mNewState);
}

auto StampToolCmd::get_name() const -> const char*
{
  return "Stamp Sequence";
}

void StampToolCmd::apply_sequence(const TileCache& cache)
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_tile_layer(mLayerId);
  layer.set_tiles(cache);
}

}  // namespace tactile
