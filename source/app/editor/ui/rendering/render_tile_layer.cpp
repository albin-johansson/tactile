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

#include "render_tile_layer.hpp"

#include "core/layers/tile_layer.hpp"
#include "core/region.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "editor/ui/rendering/render_tile.hpp"

namespace tactile::ui {

void render_tile_layer(GraphicsCtx&     graphics,
                       const Map&       map,
                       const TileLayer& layer,
                       const float      parentOpacity)
{
  graphics.set_opacity(parentOpacity * layer.get_opacity());

  const auto bounds = graphics.bounds();
  const auto endRow = bounds.end.row();
  const auto endCol = bounds.end.col();

  for (auto row = bounds.begin.row(); row < endRow; ++row) {
    for (auto col = bounds.begin.col(); col < endCol; ++col) {
      const TilePos pos{row, col};
      const auto    tile = layer.tile_at(pos);
      if (tile != empty_tile) {
        render_tile(graphics, map, tile, pos);
      }
    }
  }
}

}  // namespace tactile::ui
