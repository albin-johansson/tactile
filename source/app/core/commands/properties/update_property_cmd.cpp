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

#include "update_property_cmd.hpp"

#include <utility>  // move

#include "core/contexts/context.hpp"
#include "core/property_bundle.hpp"
#include "misc/panic.hpp"

namespace tactile {

UpdatePropertyCmd::UpdatePropertyCmd(Shared<IContext> context,
                                     std::string      name,
                                     Attribute        value)
    : mContext{std::move(context)}
    , mName{std::move(name)}
    , mNewValue{std::move(value)}
{
  if (!mContext) {
    throw TactileError{"Invalid null context!"};
  }
}

void UpdatePropertyCmd::undo()
{
  auto& props = mContext->get_props();
  props.update(mName, mOldValue.value());
  mOldValue.reset();
}

void UpdatePropertyCmd::redo()
{
  auto& props = mContext->get_props();
  mOldValue = props.at(mName);
  props.update(mName, mNewValue);
}

auto UpdatePropertyCmd::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const UpdatePropertyCmd*>(cmd)) {
    if (mContext->get_uuid() == other->mContext->get_uuid() && mName == other->mName) {
      mNewValue = other->mNewValue;
      return true;
    }
  }

  return false;
}

auto UpdatePropertyCmd::get_name() const -> const char*
{
  return "Update Property";
}

}  // namespace tactile
