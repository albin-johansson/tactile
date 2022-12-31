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

#include "rename_object.hpp"

#include <utility>  // move

#include "core/debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

RenameObject::RenameObject(Shared<Object> object, String name)
    : mObject {std::move(object)},
      mNewName {std::move(name)}
{
  if (!mObject) {
    throw TactileError {"Invalid null object!"};
  }
}

void RenameObject::undo()
{
  mObject->get_ctx().set_name(mOldName.value());
  mOldName.reset();
}

void RenameObject::redo()
{
  mOldName = mObject->get_ctx().name();
  mObject->get_ctx().set_name(mNewName);
}

auto RenameObject::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RenameObject*>(cmd)) {
    if (mObject->get_uuid() == other->mObject->get_uuid()) {
      mNewName = other->mNewName;
      return true;
    }
  }

  return false;
}

auto RenameObject::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_object;
}

}  // namespace tactile::cmd
