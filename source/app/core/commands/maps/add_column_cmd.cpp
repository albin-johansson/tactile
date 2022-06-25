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

#include "add_column_cmd.hpp"

#include "core/common/functional.hpp"
#include "core/documents/map_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddColumnCmd::AddColumnCmd(MapDocument* document)
    : ACommand{"Add Column(s)"}
    , mDocument{document}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map!"};
  }
}

void AddColumnCmd::undo()
{
  auto& map = mDocument->get_map();
  invoke_n(mColumns, [&] { map.remove_column(); });
}

void AddColumnCmd::redo()
{
  auto& map = mDocument->get_map();
  invoke_n(mColumns, [&] { map.add_column(); });
}

auto AddColumnCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const AddColumnCmd&>(cmd);
    mColumns += other.mColumns;
    return true;
  }

  return false;
}

}  // namespace tactile
