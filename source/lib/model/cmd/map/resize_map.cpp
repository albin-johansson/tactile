// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "resize_map.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

ResizeMap::ResizeMap(Shared<Map> map, const TileExtent extent)
    : mMap {std::move(map)},
      mNewExtent {extent}
{
  if (!mMap) {
    throw Exception {"Invalid null map"};
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
