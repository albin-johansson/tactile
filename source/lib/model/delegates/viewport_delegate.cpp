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

#include "common/util/lookup.hpp"
#include "components/tool.hpp"

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

}  // namespace tactile
