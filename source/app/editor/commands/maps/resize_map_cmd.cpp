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

#include "resize_map_cmd.hpp"

#include "core/common/ecs.hpp"
#include "core/components/map_info.hpp"
#include "core/systems/map_system.hpp"

namespace tactile {

ResizeMapCmd::ResizeMapCmd(RegistryRef registry, const usize nRows, const usize nCols)
    : ACommand{"Resize Map"}
    , mRegistry{registry}
    , mRows{nRows}
    , mCols{nCols}
{}

void ResizeMapCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::resize_map(registry, mPrevRows.value(), mPrevCols.value());

  if (is_lossy_resize()) {
    mCache.restore_tiles(registry);
  }
}

void ResizeMapCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto& map = ctx_get<MapInfo>(registry);
  mPrevRows = map.row_count;
  mPrevCols = map.column_count;

  if (is_lossy_resize()) {
    const auto rows = map.row_count;
    const auto cols = map.column_count;

    mCache.clear();
    mCache.save_tiles(registry,
                      TilePos::from(rows - (mPrevRows.value() - mRows), 0u),
                      TilePos::from(rows, cols));
    mCache.save_tiles(registry,
                      TilePos::from(0u, cols - (mPrevCols.value() - mCols)),
                      TilePos::from(rows, cols));
  }

  sys::resize_map(registry, mRows, mCols);
}

auto ResizeMapCmd::is_lossy_resize() const -> bool
{
  return mPrevRows > mRows || mPrevCols > mCols;
}

}  // namespace tactile
