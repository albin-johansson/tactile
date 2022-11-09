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

#include "move_layer_down.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

MoveLayerDown::MoveLayerDown(Shared<Map> map, const UUID& layer_id)
    : mMap {std::move(map)},
      mLayerId {layer_id}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void MoveLayerDown::undo()
{
  mMap->invisible_root().move_up(mLayerId);
}

void MoveLayerDown::redo()
{
  mMap->invisible_root().move_down(mLayerId);
}

auto MoveLayerDown::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.move_layer_down;
}

}  // namespace tactile::cmd
