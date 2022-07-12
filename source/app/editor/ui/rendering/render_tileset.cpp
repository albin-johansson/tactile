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

#include <imgui.h>

#include "core/common/functional.hpp"
#include "core/document/tileset_document.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile::ui {

void render_tileset(GraphicsCtx& graphics, const TilesetDocument& document)
{
  const auto& tileset = document.view_tileset();
  const auto& uvTileSize = tileset.uv_size();
  const auto  textureId = tileset.texture_id();

  const Vector2f tileSize = tileset.tile_size();
  const ImVec2   uv {uvTileSize.x, uvTileSize.y};

  invoke_mn(tileset.row_count(), tileset.column_count(), [&](int32 row, int32 col) {
    const ImVec4 source {static_cast<float>(col * tileset.tile_size().x),
                         static_cast<float>(row * tileset.tile_size().y),
                         tileSize.x,
                         tileSize.y};
    const auto   position = graphics.from_matrix_to_absolute(row, col);
    graphics.render_translated_image(textureId, source, position, uv);
  });

  if (io::get_preferences().show_grid) {
    graphics.set_line_thickness(1.0f);
    graphics.set_draw_color(cen::colors::white.with_alpha(20));
    graphics.render_translated_grid();
  }
}

}  // namespace tactile::ui
