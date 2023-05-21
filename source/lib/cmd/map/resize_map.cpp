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

#include "resize_map.hpp"

#include "core/map.hpp"
#include "model/context.hpp"
#include "model/systems/map_system.hpp"
#include "systems/language_system.hpp"

namespace tactile::cmd {

ResizeMap::ResizeMap(const Entity map_entity, const TileExtent extent)
    : mMapEntity {map_entity},
      mNewExtent {extent}
{
}

void ResizeMap::undo()
{
  auto& model = get_global_model();

  auto& map = model.get<Map>(mMapEntity);
  sys::resize_map(model, map, mOldExtent.value());

  if (is_lossy_resize()) {
    mCache.restore_tiles(model);
  }
}

void ResizeMap::redo()
{
  auto& model = get_global_model();
  auto& map = model.get<Map>(mMapEntity);

  mOldExtent = map.extent;

  if (is_lossy_resize()) {
    mCache.clear();

    const auto begin_row =
        TilePos::from(map.extent.rows - (mOldExtent->rows - mNewExtent.rows), 0u);
    const auto end_row = TilePos::from(map.extent.rows, map.extent.cols);
    mCache.save_tiles(model, mMapEntity, begin_row, end_row);

    const auto begin_col =
        TilePos::from(0u, map.extent.cols - (mOldExtent->cols - mNewExtent.cols));
    const auto end_col = TilePos::from(map.extent.rows, map.extent.cols);
    mCache.save_tiles(model, mMapEntity, begin_col, end_col);
  }

  sys::resize_map(model, map, mNewExtent);
}

auto ResizeMap::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.resize_map;
}

auto ResizeMap::is_lossy_resize() const -> bool
{
  return mOldExtent->rows > mNewExtent.rows || mOldExtent->cols > mNewExtent.cols;
}

}  // namespace tactile::cmd
