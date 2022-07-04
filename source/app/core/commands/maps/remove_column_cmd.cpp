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

#include "remove_column_cmd.hpp"

#include "core/common/functional.hpp"
#include "core/documents/map_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

RemoveColumnCmd::RemoveColumnCmd(MapDocument* document) : mDocument{document}
{
  if (!mDocument) {
    throw TactileError{"Invalid null map document!"};
  }
}

void RemoveColumnCmd::undo()
{
  auto& map = mDocument->get_map();
  invoke_n(mColumns, [&] { map.add_column(); });
  mCache.restore_tiles(map);
}

void RemoveColumnCmd::redo()
{
  auto& map = mDocument->get_map();

  const auto begin = TilePos::from(0u, map.column_count() - mColumns - 1u);
  const auto end = TilePos::from(map.row_count(), map.column_count());

  mCache.clear();
  mCache.save_tiles(map, begin, end);

  invoke_n(mColumns, [&] { map.remove_column(); });
}

auto RemoveColumnCmd::merge_with(const ICommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const RemoveColumnCmd&>(cmd);

    mColumns += other.mColumns;
    mCache.merge_with(other.mCache);

    return true;
  }

  return false;
}

auto RemoveColumnCmd::get_name() const -> const char*
{
  return mColumns == 1 ? "Remove Column" : "Remove Columns";
}

}  // namespace tactile
