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

#include "add_row.hpp"

#include <utility>  // move

#include "core/util/functional.hpp"
#include "core/debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

AddRow::AddRow(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void AddRow::undo()
{
  invoke_n(mRows, [this] { mMap->remove_row(); });
}

void AddRow::redo()
{
  invoke_n(mRows, [this] { mMap->add_row(); });
}

auto AddRow::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const AddRow*>(cmd)) {
    mRows += other->mRows;
    return true;
  }

  return false;
}

auto AddRow::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mRows == 1 ? lang.cmd.add_row : lang.cmd.add_rows;
}

}  // namespace tactile::cmd
