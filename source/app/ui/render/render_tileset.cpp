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
#include <imgui_internal.h>

#include "graphics.hpp"
#include "io/proto/preferences.hpp"
#include "model/document/tileset_document.hpp"
#include "render.hpp"
#include "ui/conversions.hpp"

namespace tactile::ui {

void render_tileset(Graphics& graphics, const TilesetDocument& document)
{
  const auto& tileset = document.view_tileset();
  const auto& texture = tileset.texture();

  const auto rendered_position = graphics.translate(ImVec2 {0, 0});
  const auto rendered_size = from_vec(texture.get_size()) * graphics.info().ratio;

  render_image(texture, rendered_position, rendered_size);

  const auto& prefs = io::get_preferences();
  if (prefs.show_grid) {
    graphics.render_infinite_grid(to_u32(prefs.grid_color));
  }

  const auto& color = to_color(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
  graphics.outline_contents(to_u32(color));
}

}  // namespace tactile::ui
