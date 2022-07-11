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

#include "move_layer_up_cmd.hpp"

#include <utility>  // move

#include "misc/panic.hpp"

namespace tactile {

MoveLayerUpCmd::MoveLayerUpCmd(Shared<Map> map, const UUID& layerId)
    : mMap {std::move(map)}
    , mLayerId {layerId}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void MoveLayerUpCmd::undo()
{
  mMap->move_layer_down(mLayerId);
}

void MoveLayerUpCmd::redo()
{
  mMap->move_layer_up(mLayerId);
}

auto MoveLayerUpCmd::get_name() const -> const char*
{
  return "Move Layer Up";
}

}  // namespace tactile
