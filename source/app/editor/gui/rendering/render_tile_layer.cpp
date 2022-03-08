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

#include "core/components/layer.hpp"
#include "core/region.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "graphics.hpp"
#include "render_tile.hpp"

namespace tactile {

void RenderTileLayer(graphics_ctx& graphics,
                     const entt::registry& registry,
                     const entt::entity layerEntity,
                     const float parentOpacity)
{
  const auto& layer = registry.get<comp::layer>(layerEntity);
  const auto& tileLayer = registry.get<comp::tile_layer>(layerEntity);

  graphics.set_opacity(parentOpacity * layer.opacity);

  const auto bounds = graphics.bounds();
  const auto endRow = bounds.end.row();
  const auto endCol = bounds.end.col();

  for (auto row = bounds.begin.row(); row < endRow; ++row) {
    for (auto col = bounds.begin.col(); col < endCol; ++col) {
      const auto tile = sys::get_tile(tileLayer, {row, col});
      if (tile != empty_tile) {
        RenderTile(graphics, registry, tile, row, col);
      }
    }
  }
}

}  // namespace tactile
