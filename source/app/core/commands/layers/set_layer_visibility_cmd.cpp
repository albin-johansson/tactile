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

#include "set_layer_visibility_cmd.hpp"

#include <utility>  // move

#include "misc/panic.hpp"

namespace tactile {

SetLayerVisibilityCmd::SetLayerVisibilityCmd(Shared<Map> map,
                                             const UUID& layerId,
                                             const bool  visible)
    : mMap {std::move(map)}
    , mLayerId {layerId}
    , mNewVisibility {visible}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void SetLayerVisibilityCmd::undo()
{
  auto& layer = mMap->view_layer(mLayerId);

  layer.set_visible(mOldVisibility.value());
  mOldVisibility.reset();
}

void SetLayerVisibilityCmd::redo()
{
  auto& layer = mMap->view_layer(mLayerId);

  mOldVisibility = layer.is_visible();
  layer.set_visible(mNewVisibility);
}

auto SetLayerVisibilityCmd::get_name() const -> const char*
{
  return mNewVisibility ? "Show Layer" : "Hide Layer";
}

}  // namespace tactile
