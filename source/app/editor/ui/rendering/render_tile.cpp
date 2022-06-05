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

#include <entt/entity/registry.hpp>
#include <imgui.h>

#include "core/common/ecs.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/model.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "editor/ui/textures.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {

void render_tile(GraphicsCtx& graphics,
                 const DocumentModel& model,
                 const MapDocument& map,
                 const TileID tile,
                 const int32 row,
                 const int32 column)
{
  TACTILE_ASSERT(tile != empty_tile);

  const auto& mapRegistry = map.get_registry();
  const auto& context = ctx_get<comp::TilesetContext>(mapRegistry);
  auto iter = context.tile_to_tileset.find(tile);

  if (iter == context.tile_to_tileset.end()) {
    return;
  }

  const auto tilesetEntity = iter->second;
  if (tilesetEntity != entt::null) {
    const auto& ref = checked_get<comp::TilesetRef>(mapRegistry, tilesetEntity);

    const auto& tileset = model.view_tileset(ref.source_tileset);
    const auto& texture = tileset.texture();
    const auto& uvSize = tileset.uv_size();

    const auto tileIndex = tileset.get_displayed_tile(tile - ref.first_id);
    const auto sourceRect = cen::cast<cen::frect>(tileset.tile_source(tileIndex));

    const ImVec4 source{sourceRect.x(),
                        sourceRect.y(),
                        sourceRect.width(),
                        sourceRect.height()};
    const ImVec2 uv{uvSize.x, uvSize.y};

    const auto position = graphics.from_matrix_to_absolute(row, column);
    graphics.render_translated_image(texture.id, source, position, uv);
  }
}

}  // namespace tactile::ui
