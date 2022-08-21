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

#include "rename_object_cmd.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/panic.hpp"

namespace tactile {

RenameObjectCmd::RenameObjectCmd(Shared<Object> object, std::string name)
    : mObject {std::move(object)}
    , mNewName {std::move(name)}
{
  if (!mObject) {
    throw TactileError {"Invalid null object!"};
  }
}

void RenameObjectCmd::undo()
{
  mObject->set_name(mOldName.value());
  mOldName.reset();
}

void RenameObjectCmd::redo()
{
  mOldName = mObject->get_name();
  mObject->set_name(mNewName);
}

auto RenameObjectCmd::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RenameObjectCmd*>(cmd)) {
    if (mObject->get_uuid() == other->mObject->get_uuid()) {
      mNewName = other->mNewName;
      return true;
    }
  }

  return false;
}

auto RenameObjectCmd::get_name() const -> std::string
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_object;
}

}  // namespace tactile
