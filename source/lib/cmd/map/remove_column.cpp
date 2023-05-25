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

#include "remove_column.hpp"

#include "common/util/functional.hpp"
#include "components/map.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/map_system.hpp"

namespace tactile::cmd {

RemoveColumn::RemoveColumn(const Entity map_entity)
    : mMapEntity {map_entity}
{
}

void RemoveColumn::undo()
{
  auto& model = get_global_model();

  auto& map = model.get<Map>(mMapEntity);
  invoke_n(mColumnCount, [&] { sys::remove_column_from_map(model, map); });

  mCache.restore_tiles(model);
}

void RemoveColumn::redo()
{
  auto& model = get_global_model();
  auto& map = model.get<Map>(mMapEntity);

  const auto begin = TilePos::from(0u, map.extent.cols - mColumnCount - 1u);
  const auto end = TilePos::from(map.extent.rows, map.extent.cols);

  mCache.clear();
  mCache.save_tiles(model, mMapEntity, begin, end);

  invoke_n(mColumnCount, [&] { sys::remove_column_from_map(model, map); });
}

auto RemoveColumn::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RemoveColumn*>(cmd)) {
    mColumnCount += other->mColumnCount;
    mCache.merge_with(other->mCache);
    return true;
  }

  return false;
}

auto RemoveColumn::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return mColumnCount == 1 ? strings.cmd.remove_column : strings.cmd.remove_columns;
}

}  // namespace tactile::cmd
