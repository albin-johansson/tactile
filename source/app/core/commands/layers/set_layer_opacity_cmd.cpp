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

#include "misc/panic.hpp"

namespace tactile {

SetLayerOpacityCmd::SetLayerOpacityCmd(Shared<ILayer> layer, const float opacity)
    : mLayer {std::move(layer)}
    , mNewOpacity {opacity}
{
  if (!mLayer) {
    throw TactileError {"Invalid null layer!"};
  }
}

void SetLayerOpacityCmd::undo()
{
  mLayer->set_opacity(mOldOpacity.value());
  mOldOpacity.reset();
}

void SetLayerOpacityCmd::redo()
{
  mOldOpacity = mLayer->get_opacity();
  mLayer->set_opacity(mNewOpacity);
}

auto SetLayerOpacityCmd::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetLayerOpacityCmd*>(cmd)) {
    if (mLayer->get_uuid() == other->mLayer->get_uuid()) {
      mNewOpacity = other->mNewOpacity;
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
