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

#include "tileset_view.hpp"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui_internal.h>

#include "core/components/map_info.hpp"
#include "core/components/viewport.hpp"
#include "core/model.hpp"
#include "editor/gui/common/mouse_tracker.hpp"
#include "editor/gui/rendering/graphics.hpp"
#include "editor/gui/rendering/render_info.hpp"
#include "editor/gui/rendering/render_tileset.hpp"
#include "io/persistence/preferences.hpp"

namespace tactile {

void update_tileset_view(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.get_active_registry();

  const auto& viewport = ctx_get<comp::Viewport>(registry);
  const auto& tileset = ctx_get<comp::Tileset>(registry);

  const auto info = get_render_info(viewport, tileset);
  update_viewport_offset(info.canvas_br - info.canvas_tl, dispatcher);

  GraphicsCtx graphics{info};

  graphics.set_draw_color(get_preferences().viewport_bg());
  graphics.clear();

  graphics.push_clip();
  render_tileset(graphics, registry);
  graphics.pop_clip();
}

}  // namespace tactile