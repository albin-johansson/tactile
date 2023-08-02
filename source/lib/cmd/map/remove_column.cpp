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

#include "core/functional/invoke.hpp"
#include "model/maps/map_components.hpp"
#include "model/maps/map_system.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

RemoveColumn::RemoveColumn(Registry* registry, const Entity map_entity)
    : mRegistry {registry},
      mMapEntity {map_entity}
{
}

void RemoveColumn::undo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  invoke_n(mColumnCount, [&] { sys::remove_column_from_map(registry, map); });

  mCache.restore_tiles(registry);
}

void RemoveColumn::redo()
{
  auto& registry = *mRegistry;
  auto& map = registry.get<Map>(mMapEntity);

  const auto begin = TilePos::from(0u, map.extent.cols - mColumnCount - 1u);
  const auto end = TilePos::from(map.extent.rows, map.extent.cols);

  mCache.clear();
  mCache.save_tiles(registry, mMapEntity, begin, end);

  invoke_n(mColumnCount, [&] { sys::remove_column_from_map(registry, map); });
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
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return mColumnCount == 1 ? strings.cmd.remove_column : strings.cmd.remove_columns;
}

}  // namespace tactile::cmd
