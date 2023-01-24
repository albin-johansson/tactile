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

#include "render_tile_layer.hpp"

#include "core/layer/tile_layer.hpp"
#include "core/region.hpp"
#include "graphics/graphics.hpp"
#include "graphics/render_tile.hpp"

namespace tactile::ui {

void render_tile_layer(Graphics& graphics,
                       const Map& map,
                       const TileLayer& layer,
                       const float parent_opacity)
{
  const auto tile_opacity = parent_opacity * layer.get_opacity();

  const auto& info = graphics.info();
  const auto end_row = info.bounds.end.row();
  const auto end_col = info.bounds.end.col();

  for (auto row = info.bounds.begin.row(); row < end_row; ++row) {
    for (auto col = info.bounds.begin.col(); col < end_col; ++col) {
      const TilePos pos {row, col};
      const auto tile_id = layer.tile_at(pos);
      if (tile_id.has_value() && tile_id != kEmptyTile) {
        render_tile(graphics, map, *tile_id, pos, tile_opacity);
      }
    }
  }
}

}  // namespace tactile::ui
