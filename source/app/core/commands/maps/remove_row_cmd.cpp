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

#include "remove_row_cmd.hpp"

#include "core/common/functional.hpp"
#include "core/components/map_info.hpp"
#include "core/documents/map_document.hpp"
#include "core/systems/map_system.hpp"
#include "misc/panic.hpp"

namespace tactile {

RemoveRowCmd::RemoveRowCmd(MapDocument* map) : ACommand{"Remove Row(s)"}, mMap{map}
{
  if (!mMap) {
    throw TactileError{"Invalid null map!"};
  }
}

void RemoveRowCmd::undo()
{
  auto& registry = mMap->get_registry();
  invoke_n(mRows, [&] { sys::add_row_to_map(registry); });
  mCache.restore_tiles(registry);
}

void RemoveRowCmd::redo()
{
  auto& registry = mMap->get_registry();
  const auto& info = mMap->info();

  const auto begin = TilePos::from(info.row_count - mRows - 1u, 0u);
  const auto end = TilePos::from(info.row_count, info.column_count);

  mCache.clear();
  mCache.save_tiles(registry, begin, end);

  invoke_n(mRows, [&] { sys::remove_row_from_map(registry); });
}

auto RemoveRowCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const RemoveRowCmd&>(cmd);

    mRows += other.mRows;
    mCache.merge_with(other.mCache);

    return true;
  }

  return false;
}

}  // namespace tactile
