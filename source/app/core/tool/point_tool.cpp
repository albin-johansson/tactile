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

#include "point_tool.hpp"

#include <centurion/mouse.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/document/map_document.hpp"
#include "core/event/tool_events.hpp"
#include "core/model.hpp"

namespace tactile {

void PointTool::on_pressed(DocumentModel&    model,
                           entt::dispatcher& dispatcher,
                           const MouseInfo&  mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_available(model)) {
    const auto& document = model.require_active_map();
    const auto& map = document.get_map();
    const auto& viewport = document.get_viewport();

    const auto ratio = viewport.get_scaling_ratio(map.tile_size());
    const auto pos = mouse.pos / ratio;

    const auto layerId = map.active_layer_id().value();
    dispatcher.enqueue<AddPointEvent>(layerId, pos);
  }
}

auto PointTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();
  return map.is_active_layer(LayerType::ObjectLayer);
}

}  // namespace tactile