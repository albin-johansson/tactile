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

#include "tile_layers.hpp"

#include "common/type/queue.hpp"

namespace tactile::sys {

void flood_tiles(TileLayer& tile_layer,
                 const TilePos& origin,
                 const TileID replacement,
                 Vector<TilePos>* affected)
{
  const auto target = tile_layer.tile_at(origin);

  if (!tile_layer.contains(origin) || target == replacement) {
    return;
  }

  Queue<TilePos> affected_positions;

  auto replace_tile = [&](const TilePos& position) {
    tile_layer.set_tile(position, replacement);

    if (affected != nullptr) {
      affected->push_back(position);
    }

    affected_positions.push(position);
  };

  replace_tile(origin);

  while (!affected_positions.empty()) {
    const auto affected_position = affected_positions.front();
    affected_positions.pop();

    auto check_position = [&](const TilePos& position) {
      if (tile_layer.tile_at(position) == target) {
        replace_tile(position);
      }
    };

    check_position(affected_position.west());
    check_position(affected_position.east());
    check_position(affected_position.south());
    check_position(affected_position.north());
  }
}

}  // namespace tactile::sys
