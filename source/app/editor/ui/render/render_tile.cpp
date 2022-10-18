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
#include "editor/ui/conversions.hpp"
#include "editor/ui/render/graphics.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {

void render_tile(Graphics& graphics,
                 const Map& map,
                 const TileID tile_id,
                 const TilePos& pos,
                 const float opacity)
{
  TACTILE_ASSERT(tile_id != empty_tile);

  const auto& tilesets = map.tileset_bundle();
  const auto tileset_id = tilesets.find_tileset(tile_id);

  if (!tileset_id) {
    return;
  }

  const auto& tileset_ref = tilesets.get_ref(*tileset_id);
  const auto& tileset = tileset_ref.view_tileset();

  const auto texture_id = tileset.texture_id();
  const auto uv = from_vec(tileset.uv_size());

  const auto tile_index = tileset.appearance_of(tileset_ref.to_index(tile_id));
  const auto& tile = tileset[tile_index];
  const auto source = from_vec(tile.source());

  const auto position = graphics.from_matrix_to_absolute(pos.row(), pos.col());
  graphics.render_translated_image(texture_id, source, position, uv, opacity);
}

}  // namespace tactile::ui
