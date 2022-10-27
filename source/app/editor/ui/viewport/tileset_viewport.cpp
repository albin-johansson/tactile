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

#include "tileset_viewport.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui_internal.h>

#include "document_viewport_offset_handler.hpp"
#include "editor/ui/render/graphics.hpp"
#include "editor/ui/render/render_info.hpp"
#include "editor/ui/render/render_tileset.hpp"
#include "editor/ui/style/colors.hpp"
#include "io/proto/preferences.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile::ui {

void show_tileset_viewport(const TilesetDocument& document, entt::dispatcher& dispatcher)
{
  const auto& tileset = document.view_tileset();
  const auto& viewport = document.get_viewport();

  const auto render_info = get_render_info(viewport, tileset);
  update_document_viewport_offset(render_info.canvas_br - render_info.canvas_tl,
                                  dispatcher);

  Graphics graphics {render_info};

  graphics.clear(color_to_u32(io::get_preferences().viewport_background));

  graphics.push_clip();
  render_tileset(graphics, document);
  graphics.pop_clip();
}

}  // namespace tactile::ui