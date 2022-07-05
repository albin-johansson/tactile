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

#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/model.hpp"
#include "editor/ui/conversions.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {

void render_tile(GraphicsCtx&   graphics,
                 const Map&     map,
                 const TileID   tileId,
                 const TilePos& pos)
{
  TACTILE_ASSERT(tileId != empty_tile);

  const auto& tilesets = map.get_tilesets();
  const auto  tilesetId = tilesets.find_tileset(tileId);

  if (!tilesetId) {
    return;
  }

  const auto& tilesetRef = tilesets.get_ref(*tilesetId);
  const auto& tileset = tilesetRef.tileset;

  const auto textureId = tileset->texture_id();
  const auto uv = from_vec(tileset->uv_size());

  const auto  tileIndex = tileset->appearance_of(tileId - tilesetRef.first_tile);
  const auto& tile = (*tileset)[tileIndex];
  const auto  source = from_vec(tile.source());

  const auto position = graphics.from_matrix_to_absolute(pos.row(), pos.col());
  graphics.render_translated_image(textureId, source, position, uv);
}

}  // namespace tactile::ui
