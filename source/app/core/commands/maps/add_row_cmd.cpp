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

#include "core/algorithms/invoke.hpp"
#include "core/documents/map_document.hpp"
#include "core/systems/map_system.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddRowCmd::AddRowCmd(MapDocument* map) : ACommand{"Add Row(s)"}, mMap{map}
{
  if (!mMap) {
    throw TactileError{"Invalid null map!"};
  }
}

void AddRowCmd::undo()
{
  auto& registry = mMap->get_registry();
  invoke_n(mRows, [&] { sys::remove_row_from_map(registry); });
}

void AddRowCmd::redo()
{
  auto& registry = mMap->get_registry();
  invoke_n(mRows, [&] { sys::add_row_to_map(registry); });
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
