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

#include "core/debug/assert.hpp"
#include "model/maps/map_components.hpp"
#include "model/maps/map_system.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

ResizeMap::ResizeMap(Registry* registry, const Entity map_entity, const TileExtent extent)
    : mRegistry {registry},
      mMapEntity {map_entity},
      mNewExtent {extent}
{
}

void ResizeMap::undo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  sys::resize_map(registry, map, mOldExtent.value());

  if (_is_lossy_resize()) {
    mCache.restore_tiles(registry);
  }
}

void ResizeMap::redo()
{
  auto& registry = *mRegistry;
  auto& map = registry.get<Map>(mMapEntity);

  mOldExtent = map.extent;

  if (_is_lossy_resize()) {
    mCache.clear();

    const auto begin_row =
        TilePos::from(map.extent.rows - (mOldExtent->rows - mNewExtent.rows), 0u);
    const auto end_row = TilePos::from(map.extent.rows, map.extent.cols);
    mCache.save_tiles(registry, mMapEntity, begin_row, end_row);

    const auto begin_col =
        TilePos::from(0u, map.extent.cols - (mOldExtent->cols - mNewExtent.cols));
    const auto end_col = TilePos::from(map.extent.rows, map.extent.cols);
    mCache.save_tiles(registry, mMapEntity, begin_col, end_col);
  }

  sys::resize_map(registry, map, mNewExtent);
}

auto ResizeMap::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.resize_map;
}

auto ResizeMap::_is_lossy_resize() const -> bool
{
  TACTILE_ASSERT(mOldExtent.has_value());
  return mOldExtent->rows > mNewExtent.rows || mOldExtent->cols > mNewExtent.cols;
}

}  // namespace tactile::cmd
