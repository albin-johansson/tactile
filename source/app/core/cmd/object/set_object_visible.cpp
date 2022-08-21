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

#include "set_object_visible.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile::cmd {

SetObjectVisible::SetObjectVisible(Shared<Object> object, const bool visible)
    : mObject {std::move(object)}
    , mNewVisibility {visible}
{
  if (!mObject) {
    throw TactileError {"Invalid null object!"};
  }
}

void SetObjectVisible::undo()
{
  mObject->set_visible(mOldVisibility.value());
  mOldVisibility.reset();
}

void SetObjectVisible::redo()
{
  mOldVisibility = mObject->is_visible();
  mObject->set_visible(mNewVisibility);
}

auto SetObjectVisible::get_name() const -> std::string
{
  const auto& lang = get_current_language();
  return mNewVisibility ? lang.cmd.show_object : lang.cmd.hide_object;
}

}  // namespace tactile::cmd
