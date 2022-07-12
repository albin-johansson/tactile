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

#include "move_object_cmd.hpp"

#include <utility>  // move

#include "misc/panic.hpp"

namespace tactile {

MoveObjectCmd::MoveObjectCmd(Shared<Object>  object,
                             const Vector2f& previous,
                             const Vector2f& updated)
    : mObject {std::move(object)}
    , mPreviousPos {previous}
    , mUpdatedPos {updated}
{
  if (!mObject) {
    throw TactileError {"Invalid null object!"};
  }
}

void MoveObjectCmd::undo()
{
  mObject->set_pos(mPreviousPos);
}

void MoveObjectCmd::redo()
{
  mObject->set_pos(mUpdatedPos);
}

auto MoveObjectCmd::get_name() const -> const char*
{
  return "Move Object";
}

}  // namespace tactile
