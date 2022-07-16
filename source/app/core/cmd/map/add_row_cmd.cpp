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

#include "add_row_cmd.hpp"

#include <utility>  // move

#include "core/common/functional.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddRowCmd::AddRowCmd(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void AddRowCmd::undo()
{
  invoke_n(mRows, [this] { mMap->remove_row(); });
}

void AddRowCmd::redo()
{
  invoke_n(mRows, [this] { mMap->add_row(); });
}

auto AddRowCmd::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const AddRowCmd*>(cmd)) {
    mRows += other->mRows;
    return true;
  }

  return false;
}

auto AddRowCmd::get_name() const -> const char*
{
  return mRows == 1 ? "Add Row" : "Add Rows";
}

}  // namespace tactile
