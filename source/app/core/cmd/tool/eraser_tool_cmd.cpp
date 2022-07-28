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

#include "core/layer/tile_layer.hpp"
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile {

EraserToolCmd::EraserToolCmd(Shared<Map> map, const UUID& layerId, TileCache oldState)
    : mMap {std::move(map)}
    , mLayerId {layerId}
    , mOldState {std::move(oldState)}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void EraserToolCmd::undo()
{
  auto& layer = mMap->view_tile_layer(mLayerId);
  layer.set_tiles(mOldState);
}

void EraserToolCmd::redo()
{
  auto& layer = mMap->view_tile_layer(mLayerId);

  for (const auto& [position, _] : mOldState) {
    layer.set_tile(position, empty_tile);
  }
}

auto EraserToolCmd::get_name() const -> std::string
{
  const auto& lang = get_current_language();
  return lang.cmd.eraser_tool;
}

}  // namespace tactile
