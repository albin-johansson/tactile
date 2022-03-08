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

add_row_cmd::add_row_cmd(registry_ref registry)
    : command_base{"Add Row(s)"}
    , mRegistry{registry}
{}

void add_row_cmd::undo()
{
  invoke_n(mRows, [this] { sys::remove_row_from_map(mRegistry); });
}

void add_row_cmd::redo()
{
  invoke_n(mRows, [this] { sys::add_row_to_map(mRegistry); });
}

auto add_row_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const add_row_cmd&>(cmd);
    mRows += other.mRows;
    return true;
  }

  return false;
}

}  // namespace tactile
