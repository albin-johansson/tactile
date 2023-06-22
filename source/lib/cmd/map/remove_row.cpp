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

#include "remove_row.hpp"

#include "common/util/functional.hpp"
#include "model/maps/map_components.hpp"
#include "model/maps/map_system.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

RemoveRow::RemoveRow(Model* model, const Entity map_entity)
    : mModel {model},
      mMapEntity {map_entity}
{
}

void RemoveRow::undo()
{
  auto& model = *mModel;

  auto& map = model.get<Map>(mMapEntity);
  invoke_n(mRowCount, [&] { sys::add_row_to_map(model, map); });

  mCache.restore_tiles(model);
}

void RemoveRow::redo()
{
  auto& model = *mModel;
  auto& map = model.get<Map>(mMapEntity);

  const auto begin = TilePos::from(map.extent.rows - mRowCount - 1u, 0u);
  const auto end = TilePos::from(map.extent.rows, map.extent.cols);

  mCache.clear();
  mCache.save_tiles(model, mMapEntity, begin, end);

  invoke_n(mRowCount, [&] { sys::remove_row_from_map(model, map); });
}

auto RemoveRow::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RemoveRow*>(cmd)) {
    mRowCount += other->mRowCount;
    mCache.merge_with(other->mCache);
    return true;
  }

  return false;
}

auto RemoveRow::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return mRowCount == 1 ? strings.cmd.remove_row : strings.cmd.remove_rows;
}

}  // namespace tactile::cmd
