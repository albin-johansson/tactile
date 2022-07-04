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

#include "core/documents/map_document.hpp"
#include "core/layers/tile_layer.hpp"
#include "misc/panic.hpp"

namespace tactile {

EraserToolCmd::EraserToolCmd(MapDocument* document,
                             const UUID&  layerId,
                             TileCache    oldState)
    : mDocument{document}
    , mLayerId{layerId}
    , mOldState{std::move(oldState)}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void EraserToolCmd::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_tile_layer(mLayerId);
  layer.set_tiles(mOldState);
}

void EraserToolCmd::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_tile_layer(mLayerId);

  for (const auto& [position, _] : mOldState) {
    layer.set_tile(position, empty_tile);
  }
}

auto EraserToolCmd::get_name() const -> const char*
{
  return "Eraser Sequence";
}

}  // namespace tactile
