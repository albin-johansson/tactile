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

#include "render_tile.hpp"

#include "core/map.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/debug/assert.hpp"
#include "graphics/graphics.hpp"
#include "ui/conversions.hpp"

namespace tactile::ui {

void render_tile(Graphics& graphics,
                 const Map& map,
                 const TileID tile_id,
                 const TilePos& pos,
                 const float opacity)
{
  TACTILE_ASSERT(tile_id != empty_tile);
  const auto& tilesets = map.tileset_bundle();

  if (const auto tileset_id = tilesets.find_tileset(tile_id)) {
    const auto& tileset_ref = tilesets.get_ref(*tileset_id);
    const auto& tileset = tileset_ref.view_tileset();

    const auto tile_index = tileset.appearance_of(tileset_ref.to_index(tile_id));
    const auto tileset_pos = TilePos::from_index(tile_index, tileset.column_count());

    const auto absolute_position = graphics.from_matrix_to_absolute(pos);
    const auto rendered_position = graphics.translate(absolute_position);
    graphics.render_tile(tileset, tileset_pos, rendered_position, opacity_cast(opacity));
  }
}

}  // namespace tactile::ui
