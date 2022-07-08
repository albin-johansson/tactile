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

#include "set_object_visible_cmd.hpp"

#include <utility>  // move

#include "misc/panic.hpp"

namespace tactile {

SetObjectVisibleCmd::SetObjectVisibleCmd(Shared<Object> object, const bool visible)
    : mObject{std::move(object)}
    , mNewVisibility{visible}
{
  if (!mObject) {
    throw TactileError{"Invalid null object!"};
  }
}

void SetObjectVisibleCmd::undo()
{
  mObject->set_visible(mOldVisibility.value());
  mOldVisibility.reset();
}

void SetObjectVisibleCmd::redo()
{
  mOldVisibility = mObject->is_visible();
  mObject->set_visible(mNewVisibility);
}

auto SetObjectVisibleCmd::get_name() const -> const char*
{
  return mNewVisibility ? "Show Object" : "Hide Object";
}

}  // namespace tactile
