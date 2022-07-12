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

#include <utility>  // move

#include "core/document/map_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

ResizeMapCmd::ResizeMapCmd(Shared<Map> map, const usize nRows, const usize nCols)
    : mMap {std::move(map)}
    , mRows {nRows}
    , mCols {nCols}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void ResizeMapCmd::undo()
{
  mMap->resize(mPrevRows.value(), mPrevCols.value());

  if (is_lossy_resize()) {
    mCache.restore_tiles(*mMap);
  }
}

void ResizeMapCmd::redo()
{
  mPrevRows = mMap->row_count();
  mPrevCols = mMap->column_count();

  if (is_lossy_resize()) {
    const auto rows = mMap->row_count();
    const auto cols = mMap->column_count();

    mCache.clear();
    mCache.save_tiles(*mMap,
                      TilePos::from(rows - (mPrevRows.value() - mRows), 0u),
                      TilePos::from(rows, cols));
    mCache.save_tiles(*mMap,
                      TilePos::from(0u, cols - (mPrevCols.value() - mCols)),
                      TilePos::from(rows, cols));
  }

  mMap->resize(mRows, mCols);
}

auto ResizeMapCmd::get_name() const -> const char*
{
  return "Resize Map";
}

auto ResizeMapCmd::is_lossy_resize() const -> bool
{
  return mPrevRows > mRows || mPrevCols > mCols;
}

}  // namespace tactile
