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

#include "model/systems/language_system.hpp"

namespace tactile::cmd {

FixMapTiles::FixMapTiles(Model* model, const Entity map_entity)
    : mModel {model},
      mMapEntity {map_entity}
{
}

void FixMapTiles::undo()
{
  auto& model = *mModel;

  sys::restore_tiles_in_map(model, mInvalidLayerTiles);
  mInvalidLayerTiles.clear();
}

void FixMapTiles::redo()
{
  auto& model = *mModel;

  auto& map = model.get<Map>(mMapEntity);
  mInvalidLayerTiles = sys::fix_tiles_in_map(model, map);
}

auto FixMapTiles::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.fix_map_tiles;
}

}  // namespace tactile::cmd
