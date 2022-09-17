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

#include "core/document/tileset_document.hpp"
#include "core/util/functional.hpp"
#include "editor/ui/conversions.hpp"
#include "editor/ui/render/graphics.hpp"
#include "io/proto/preferences.hpp"

namespace tactile::ui {

void render_tileset(Graphics& graphics, const TilesetDocument& document)
{
  const auto& tileset = document.view_tileset();

  const Float2 tile_size = tileset.tile_size();
  const auto uv = from_vec(tileset.uv_size());

  invoke_mn(tileset.row_count(), tileset.column_count(), [&](int32 row, int32 col) {
    const ImVec4 source {static_cast<float>(col * tileset.tile_size().x),
                         static_cast<float>(row * tileset.tile_size().y),
                         tile_size.x,
                         tile_size.y};
    const auto position = graphics.from_matrix_to_absolute(row, col);
    graphics.render_translated_image(tileset.texture_id(), source, position, uv);
  });

  if (io::get_preferences().show_grid) {
    graphics.render_translated_grid(IM_COL32(0xFF, 0xFF, 0xFF, 20));
  }
}

}  // namespace tactile::ui
