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

#include "set_tile_format_compression.hpp"

#include "core/map.hpp"
#include "model/context.hpp"
#include "systems/language_system.hpp"

namespace tactile::cmd {

SetTileFormatCompression::SetTileFormatCompression(const Entity map_entity,
                                                   const TileCompression compression)
    : mMapEntity {map_entity},
      mNewCompression {compression}
{
}

void SetTileFormatCompression::undo()
{
  auto& model = get_global_model();
  auto& format = model.get<TileFormat>(mMapEntity);

  format.compression = mOldCompression.value();
  mOldCompression.reset();
}

void SetTileFormatCompression::redo()
{
  auto& model = get_global_model();
  auto& format = model.get<TileFormat>(mMapEntity);

  mOldCompression = format.compression;
  format.compression = mNewCompression;
}

auto SetTileFormatCompression::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.set_tile_format_compression;
}

}  // namespace tactile::cmd