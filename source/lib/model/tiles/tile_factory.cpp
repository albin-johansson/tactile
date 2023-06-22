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

#include "tile_factory.hpp"

#include "common/debug/assert.hpp"
#include "common/tile_matrix.hpp"
#include "model/components/context.hpp"
#include "model/systems/validation_system.hpp"
#include "model/tiles/tile_components.hpp"

namespace tactile::sys {

auto create_tile(Model& model, const Tileset& tileset, const TileIndex tile_index)
    -> Entity
{
  const auto [row, col] = to_matrix_coords(tile_index, tileset.column_count);

  const auto tile_entity = model.create_entity();

  auto& context = model.add<Context>(tile_entity);
  context.name = "Tile";

  const Int2 tile_position {col * tileset.tile_size.x, row * tileset.tile_size.y};

  auto& tile = model.add<Tile>(tile_entity);
  tile.index = tile_index;
  tile.source = Int4 {tile_position, tileset.tile_size};

  TACTILE_ASSERT(is_tile_entity(model, tile_entity));
  return tile_entity;
}

}  // namespace tactile::sys
