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

#include "resize_map.hpp"

#include <utility>  // move

#include "common/debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

ResizeMap::ResizeMap(Shared<Map> map, const TileExtent extent)
    : mMap {std::move(map)},
      mNewExtent {extent}
{
  if (!mMap) {
    throw TactileError {"Invalid null map"};
  }
}

void ResizeMap::undo()
{
  mMap->resize(mOldExtent.value());

  if (is_lossy_resize()) {
    mCache.restore_tiles(*mMap);
  }
}

void ResizeMap::redo()
{
  mOldExtent = mMap->get_extent();

  if (is_lossy_resize()) {
    const auto extent = mMap->get_extent();

    mCache.clear();
    mCache.save_tiles(
        *mMap,
        TilePos::from(extent.rows - (mOldExtent->rows - mNewExtent.rows), 0u),
        TilePos::from(extent.rows, extent.cols));
    mCache.save_tiles(
        *mMap,
        TilePos::from(0u, extent.cols - (mOldExtent->cols - mNewExtent.cols)),
        TilePos::from(extent.rows, extent.cols));
  }

  mMap->resize(mNewExtent);
}

auto ResizeMap::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.resize_map;
}

auto ResizeMap::is_lossy_resize() const -> bool
{
  return mOldExtent->rows > mNewExtent.rows || mOldExtent->cols > mNewExtent.cols;
}

}  // namespace tactile::cmd
