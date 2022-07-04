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

#include "set_layer_opacity_cmd.hpp"

#include "core/documents/map_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

SetLayerOpacityCmd::SetLayerOpacityCmd(MapDocument* document,
                                       const UUID&  layerId,
                                       const float  opacity)
    : mDocument{document}
    , mLayerId{layerId}
    , mOpacity{opacity}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void SetLayerOpacityCmd::undo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_layer(mLayerId);

  layer.set_opacity(mPreviousOpacity.value());
  mPreviousOpacity.reset();
}

void SetLayerOpacityCmd::redo()
{
  auto& map = mDocument->get_map();
  auto& layer = map.view_layer(mLayerId);

  mPreviousOpacity = layer.get_opacity();
  layer.set_opacity(mOpacity);
}

auto SetLayerOpacityCmd::merge_with(const ICommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const SetLayerOpacityCmd&>(cmd);
    if (mLayerId == other.mLayerId) {
      mOpacity = other.mOpacity;
      return true;
    }
  }

  return false;
}

auto SetLayerOpacityCmd::get_name() const -> const char*
{
  return "Set Layer Opacity";
}

}  // namespace tactile
