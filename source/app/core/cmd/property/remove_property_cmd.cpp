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

#include "remove_property_cmd.hpp"

#include <utility>  // move

#include "core/ctx/context.hpp"
#include "core/ctx/property_bundle.hpp"
#include "misc/panic.hpp"

namespace tactile {

RemovePropertyCmd::RemovePropertyCmd(Shared<IContext> context, std::string name)
    : mContext {std::move(context)}
    , mName {std::move(name)}
{
  if (!mContext) {
    throw TactileError {"Invalid null context!"};
  }
}

void RemovePropertyCmd::undo()
{
  auto& props = mContext->get_props();
  props.add(mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void RemovePropertyCmd::redo()
{
  auto& props = mContext->get_props();
  mPreviousValue = props.at(mName);
  props.remove(mName);
}

auto RemovePropertyCmd::get_name() const -> std::string
{
  return "Remove Property";
}

}  // namespace tactile
