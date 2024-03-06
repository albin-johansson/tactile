/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "move_object.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

MoveObject::MoveObject(Shared<Object> object,
                       const Float2& previous,
                       const Float2& updated)
    : mObject {std::move(object)},
      mPreviousPos {previous},
      mUpdatedPos {updated}
{
  if (!mObject) {
    throw Exception {"Invalid null object!"};
  }
}

void MoveObject::undo()
{
  mObject->set_pos(mPreviousPos);
}

void MoveObject::redo()
{
  mObject->set_pos(mUpdatedPos);
}

auto MoveObject::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.move_object;
}

}  // namespace tactile::cmd
