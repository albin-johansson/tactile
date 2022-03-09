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

#include "core/algorithms/invoke_n.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

AddRowCmd::AddRowCmd(RegistryRef registry) : ACommand{"Add Row(s)"}, mRegistry{registry}
{}

void AddRowCmd::undo()
{
  invoke_n(mRows, [this] { sys::remove_row_from_map(mRegistry); });
}

void AddRowCmd::redo()
{
  invoke_n(mRows, [this] { sys::add_row_to_map(mRegistry); });
}

auto AddRowCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const AddRowCmd&>(cmd);
    mRows += other.mRows;
    return true;
  }

  return false;
}

}  // namespace tactile
