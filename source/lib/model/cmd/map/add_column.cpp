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

#include "add_column.hpp"

#include <utility>  // move

#include "common/util/functional.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

AddColumn::AddColumn(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw Exception {"Invalid null map"};
  }
}

void AddColumn::undo()
{
  invoke_n(mColumnCount, [this] { mMap->remove_column(); });
}

void AddColumn::redo()
{
  invoke_n(mColumnCount, [this] { mMap->add_column(); });
}

auto AddColumn::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const AddColumn*>(cmd)) {
    mColumnCount += other->mColumnCount;
    return true;
  }

  return false;
}

auto AddColumn::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mColumnCount == 1 ? lang.cmd.add_column : lang.cmd.add_columns;
}

}  // namespace tactile::cmd
