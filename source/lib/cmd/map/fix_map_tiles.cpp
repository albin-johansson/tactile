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

#include "fix_map_tiles.hpp"

#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile::cmd {

FixMapTiles::FixMapTiles(Registry* registry, const Entity map_entity)
    : mRegistry {registry},
      mMapEntity {map_entity}
{
}

void FixMapTiles::undo()
{
  auto& registry = *mRegistry;

  sys::restore_tiles_in_map(registry, mInvalidLayerTiles);
  mInvalidLayerTiles.clear();
}

void FixMapTiles::redo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  mInvalidLayerTiles = sys::fix_tiles_in_map(registry, map);
}

auto FixMapTiles::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.fix_map_tiles;
}

}  // namespace tactile::cmd
