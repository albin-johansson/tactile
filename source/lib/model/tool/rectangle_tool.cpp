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

#include "rectangle_tool.hpp"

#include <glm/common.hpp>

#include "common/type/math.hpp"
#include "components/document.hpp"
#include "components/map.hpp"
#include "core/layer.hpp"
#include "core/viewport.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"

namespace tactile {

void RectangleTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void RectangleTool::on_disabled(Model& model, Dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void RectangleTool::on_exited(Model& model, Dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void RectangleTool::on_pressed(Model& model, Dispatcher&, const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      is_available(model)) {
    auto& stroke = mStroke.emplace();
    stroke.start = mouse.pos;
    stroke.current = stroke.start;
  }
}

void RectangleTool::on_dragged(Model& model, Dispatcher&, const MouseInfo& mouse)
{
  if (mStroke && mouse.button == cen::mouse_button::left && is_available(model)) {
    mStroke->current = mouse.pos;
  }
}

void RectangleTool::on_released(Model& model,
                                Dispatcher& dispatcher,
                                const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto RectangleTool::is_available(const Model& model) const -> bool
{
  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  return map.active_layer != kNullEntity && model.has<ObjectLayer>(map.active_layer);
}

auto RectangleTool::get_stroke() const -> const Maybe<CurrentRectangleStroke>&
{
  return mStroke;
}

void RectangleTool::maybe_emit_event(Model& model, Dispatcher& dispatcher)
{
  if (mStroke) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& document_viewport = model.get<Viewport>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    const auto scaling_ratio = document_viewport.scaling_ratio(map.tile_size);
    const auto position = glm::min(mStroke->start, mStroke->current) / scaling_ratio;
    const auto size = glm::abs(mStroke->current - mStroke->start) / scaling_ratio;

    if (size.x != 0 && size.y != 0) {
      dispatcher.enqueue<AddRectangleEvent>(map.active_layer, position, size);
    }

    mStroke.reset();
  }
}

}  // namespace tactile