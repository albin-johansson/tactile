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

#include "set_layer_opacity.hpp"

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

SetLayerOpacity::SetLayerOpacity(Shared<Layer> layer, const float opacity)
    : mLayer {std::move(layer)}
    , mNewOpacity {opacity}
{
  if (!mLayer) {
    throw TactileError {"Invalid null layer!"};
  }
}

void SetLayerOpacity::undo()
{
  mLayer->set_opacity(mOldOpacity.value());
  mOldOpacity.reset();
}

void SetLayerOpacity::redo()
{
  mOldOpacity = mLayer->opacity();
  mLayer->set_opacity(mNewOpacity);
}

auto SetLayerOpacity::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetLayerOpacity*>(cmd)) {
    if (mLayer->uuid() == other->mLayer->uuid()) {
      mNewOpacity = other->mNewOpacity;
      return true;
    }
  }

  return false;
}

auto SetLayerOpacity::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.set_layer_opacity;
}

}  // namespace tactile::cmd