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

#include <entt/entity/registry.hpp>

#include "core/components/layers.hpp"
#include "core/region.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "editor/gui/rendering/graphics.hpp"
#include "editor/gui/rendering/render_tile.hpp"

namespace tactile {

void render_tile_layer(GraphicsCtx& graphics,
                       const entt::registry& registry,
                       const entt::entity layerEntity,
                       const float parentOpacity)
{
  const auto& layer = sys::checked_get<comp::Layer>(registry, layerEntity);
  const auto& tileLayer = sys::checked_get<comp::TileLayer>(registry, layerEntity);

  graphics.set_opacity(parentOpacity * layer.opacity);

  const auto bounds = graphics.bounds();
  const auto endRow = bounds.end.row();
  const auto endCol = bounds.end.col();

  for (auto row = bounds.begin.row(); row < endRow; ++row) {
    for (auto col = bounds.begin.col(); col < endCol; ++col) {
      const auto tile = sys::get_tile(tileLayer, {row, col});
      if (tile != empty_tile) {
        render_tile(graphics, registry, tile, row, col);
      }
    }
  }
}

}  // namespace tactile
