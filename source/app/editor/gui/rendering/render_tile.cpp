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

#include <imgui.h>

#include "core/components/texture.hpp"
#include "core/components/uv_tile_size.hpp"
#include "core/systems/tileset_system.hpp"
#include "editor/gui/textures.hpp"
#include "graphics.hpp"

namespace tactile {

void RenderTile(graphics_ctx& graphics,
                const entt::registry& registry,
                const tile_id tile,
                const int32 row,
                const int32 column)
{
  const auto& context = registry.ctx<comp::tileset_context>();
  auto iter = context.tile_to_tileset.find(tile);

  if (iter == context.tile_to_tileset.end()) {
    return;
  }

  const auto tilesetEntity = iter->second;
  if (tilesetEntity != entt::null) {
    const auto& texture = registry.get<comp::texture>(tilesetEntity);
    const auto& uvTileSize = registry.get<comp::uv_tile_size>(tilesetEntity);

    const auto tileToRender = sys::get_tile_to_render(registry, tilesetEntity, tile);
    const auto& sourceRect = sys::get_source_rect(registry, tilesetEntity, tileToRender);

    const ImVec4 source{static_cast<float>(sourceRect.x()),
                        static_cast<float>(sourceRect.y()),
                        static_cast<float>(sourceRect.width()),
                        static_cast<float>(sourceRect.height())};
    const ImVec2 uv{uvTileSize.width, uvTileSize.height};

    const auto position = graphics.from_matrix_to_absolute(row, column);
    graphics.render_translated_image(texture.id, source, position, uv);
  }
}

}  // namespace tactile
