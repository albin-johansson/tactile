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

#include "viewport_delegate.hpp"

#include <cmath>  // round

#include "common/util/lookup.hpp"
#include "model/components/document.hpp"
#include "model/components/tool.hpp"
#include "model/documents/document_system.hpp"
#include "model/systems/viewport_system.hpp"

namespace tactile {

void on_viewport_mouse_pressed(Model& model,
                               Dispatcher& dispatcher,
                               const ViewportMousePressedEvent& event)
{
  auto& tool_context = model.get<ToolContext>();

  if (tool_context.active_tool.has_value()) {
    const auto tool_entity = lookup_in(tool_context.tools, *tool_context.active_tool);
    const auto& tool = model.get<Tool>(tool_entity);

    if (tool.on_pressed) {
      tool.on_pressed(model, tool_entity, event.mouse_info, dispatcher);
    }
  }
}

void on_viewport_mouse_dragged(Model& model,
                               Dispatcher& dispatcher,
                               const ViewportMouseDraggedEvent& event)
{
  auto& tool_context = model.get<ToolContext>();

  if (tool_context.active_tool.has_value()) {
    const auto tool_entity = lookup_in(tool_context.tools, *tool_context.active_tool);
    const auto& tool = model.get<Tool>(tool_entity);

    if (tool.on_dragged) {
      tool.on_dragged(model, tool_entity, event.mouse_info, dispatcher);
    }
  }
}

void on_viewport_mouse_released(Model& model,
                                Dispatcher& dispatcher,
                                const ViewportMouseReleasedEvent& event)
{
  auto& tool_context = model.get<ToolContext>();

  if (tool_context.active_tool.has_value()) {
    const auto tool_entity = lookup_in(tool_context.tools, *tool_context.active_tool);
    const auto& tool = model.get<Tool>(tool_entity);

    if (tool.on_released) {
      tool.on_released(model, tool_entity, event.mouse_info, dispatcher);
    }
  }
}

void on_viewport_mouse_entered(Model& model,
                               Dispatcher& dispatcher,
                               const ViewportMouseEnteredEvent&)
{
  auto& tool_context = model.get<ToolContext>();

  if (tool_context.active_tool.has_value()) {
    const auto tool_entity = lookup_in(tool_context.tools, *tool_context.active_tool);
    const auto& tool = model.get<Tool>(tool_entity);

    if (tool.on_entered) {
      tool.on_entered(model, tool_entity, dispatcher);
    }
  }
}

void on_viewport_mouse_exited(Model& model,
                              Dispatcher& dispatcher,
                              const ViewportMouseExitedEvent&)
{
  auto& tool_context = model.get<ToolContext>();

  if (tool_context.active_tool.has_value()) {
    const auto tool_entity = lookup_in(tool_context.tools, *tool_context.active_tool);
    const auto& tool = model.get<Tool>(tool_entity);

    if (tool.on_exited) {
      tool.on_exited(model, tool_entity, dispatcher);
    }
  }
}

void on_center_viewport(Model& model, const CenterViewportEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  const auto& viewport_info = model.get<DynamicViewportInfo>(event.viewport);

  if (viewport_info.total_size.has_value() && viewport_info.content_size.has_value()) {
    const auto total_size = *viewport_info.total_size;
    const auto content_size = *viewport_info.content_size;

    const auto raw_offset = ((total_size - content_size) / 2.0f) - viewport.offset;
    const Float2 rounded_offset {std::round(raw_offset.x), std::round(raw_offset.y)};

    on_offset_viewport(model, OffsetViewportEvent {event.viewport, rounded_offset});
  }
}

void on_reset_viewport_zoom(Model& model, const ResetViewportZoomEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  sys::reset_viewport_zoom(viewport);
}

void on_increase_viewport_zoom(Model& model, const IncreaseViewportZoomEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  sys::increase_viewport_zoom(viewport, event.anchor_pos);
}

void on_decrease_viewport_zoom(Model& model, const DecreaseViewportZoomEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  sys::decrease_viewport_zoom(viewport, event.anchor_pos);
}

void on_offset_viewport(Model& model, const OffsetViewportEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  sys::offset_viewport(viewport, event.delta);
}

void on_set_viewport_limits(Model& model, const SetViewportLimitsEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  viewport.limits = ViewportLimits {event.min_offset, event.max_offset};
}

void on_set_dynamic_viewport_info(Model& model, const SetDynamicViewportInfoEvent& event)
{
  auto& dynamic_info = model.get<DynamicViewportInfo>(event.viewport);
  dynamic_info = event.info;
}

void on_pan_viewport_up(Model& model, const PanViewportUpEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  viewport.offset.y += viewport.tile_size.y;
}

void on_pan_viewport_down(Model& model, const PanViewportDownEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  viewport.offset.y -= viewport.tile_size.y;
}

void on_pan_viewport_left(Model& model, const PanViewportLeftEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  viewport.offset.x += viewport.tile_size.x;
}

void on_pan_viewport_right(Model& model, const PanViewportRightEvent& event)
{
  auto& viewport = model.get<Viewport>(event.viewport);
  viewport.offset.x -= viewport.tile_size.x;
}

}  // namespace tactile
