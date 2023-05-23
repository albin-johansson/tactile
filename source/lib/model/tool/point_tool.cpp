/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "components/document.hpp"
#include "components/map.hpp"
#include "core/layer.hpp"
#include "core/viewport.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"

namespace tactile {

void PointTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void PointTool::on_pressed(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse)
{
  if (mouse.is_within_contents &&                 //
      mouse.button == cen::mouse_button::left &&  //
      is_available(model)) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& document_viewport = model.get<Viewport>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    const auto scaling_ratio = document_viewport.scaling_ratio(map.tile_size);
    const auto position = mouse.pos / scaling_ratio;

    dispatcher.enqueue<AddPointEvent>(map.active_layer, position);
  }
}

auto PointTool::is_available(const Model& model) const -> bool
{
  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  return map.active_layer != kNullEntity && model.has<ObjectLayer>(map.active_layer);
}

}  // namespace tactile