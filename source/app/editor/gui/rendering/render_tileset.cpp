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

#include "render_tileset.hpp"

#include <entt/entity/registry.hpp>
#include <imgui.h>

#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "editor/gui/rendering/graphics.hpp"

namespace tactile {

void render_tileset(GraphicsCtx& graphics, const entt::registry& registry)
{
  const auto& ctx = registry.ctx();

  const auto& tileset = ctx.at<comp::Tileset>();
  const auto& texture = ctx.at<comp::Texture>();
  const auto& uvTileSize = ctx.at<comp::UvTileSize>();

  const auto tw = static_cast<float>(tileset.tile_width);
  const auto th = static_cast<float>(tileset.tile_height);
  const ImVec2 uv{uvTileSize.width, uvTileSize.height};

  for (int32 row = 0; row < tileset.row_count; ++row) {
    for (int32 col = 0; col < tileset.column_count; ++col) {
      const ImVec4 source{static_cast<float>(col * tileset.tile_width),
                          static_cast<float>(row * tileset.tile_height),
                          tw,
                          th};
      const auto position = graphics.from_matrix_to_absolute(row, col);
      graphics.render_translated_image(texture.id, source, position, uv);
    }
  }
}

}  // namespace tactile
