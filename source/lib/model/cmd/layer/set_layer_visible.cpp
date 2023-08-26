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

#include "set_layer_visible.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/error.hpp"

namespace tactile::cmd {

SetLayerVisible::SetLayerVisible(Shared<Map> map,
                                 const UUID& layer_id,
                                 const bool visible)
    : mMap {std::move(map)},
      mLayerId {layer_id},
      mNewVisibility {visible}
{
  if (!mMap) {
    throw Error {"Invalid null map!"};
  }
}

void SetLayerVisible::undo()
{
  auto& layer = mMap->get_invisible_root().get_layer(mLayerId);

  layer.set_visible(mOldVisibility.value());
  mOldVisibility.reset();
}

void SetLayerVisible::redo()
{
  auto& layer = mMap->get_invisible_root().get_layer(mLayerId);

  mOldVisibility = layer.is_visible();
  layer.set_visible(mNewVisibility);
}

auto SetLayerVisible::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mNewVisibility ? lang.cmd.show_layer : lang.cmd.hide_layer;
}

}  // namespace tactile::cmd
