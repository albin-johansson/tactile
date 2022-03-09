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

#include "tool_system.hpp"

#include "bucket_tool_system.hpp"
#include "core/components/tools.hpp"
#include "ellipse_tool_system.hpp"
#include "eraser_tool_system.hpp"
#include "object_selection_tool_system.hpp"
#include "point_tool_system.hpp"
#include "rectangle_tool_system.hpp"
#include "stamp_tool_system.hpp"

namespace tactile::sys {

void select_tool(entt::registry& registry,
                 entt::dispatcher& dispatcher,
                 const ToolType tool)
{
  auto& active = registry.ctx<comp::ActiveTool>();

  switch (active.tool) {
    case ToolType::stamp:
      on_stamp_tool_disabled(dispatcher);
      break;

    case ToolType::eraser:
      on_eraser_tool_disabled(dispatcher);
      break;

    case ToolType::rectangle:
      on_rectangle_tool_disabled(registry, dispatcher);
      break;

    case ToolType::ellipse:
      on_ellipse_tool_disabled(registry, dispatcher);
      break;

    case ToolType::bucket:
    case ToolType::object_selection:
    case ToolType::point:
      [[fallthrough]];
    case ToolType::none:
      break;
  }

  active.tool = (active.tool == tool) ? ToolType::none : tool;
}

void on_tool_entered(entt::registry& registry, entt::dispatcher&)
{
  const auto& active = registry.ctx<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::stamp:
    case ToolType::eraser:
    case ToolType::bucket:
    case ToolType::object_selection:
    case ToolType::rectangle:
    case ToolType::ellipse:
    case ToolType::point:
      [[fallthrough]];
    case ToolType::none:
      break;
  }
}

void on_tool_exited(entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto& active = registry.ctx<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::stamp:
      on_stamp_tool_exited(dispatcher);
      break;

    case ToolType::eraser:
      on_eraser_tool_exited(dispatcher);
      break;

    case ToolType::object_selection:
      on_object_selection_tool_exited(registry, dispatcher);
      break;

    case ToolType::rectangle:
      on_rectangle_tool_exited(registry, dispatcher);
      break;

    case ToolType::ellipse:
      on_ellipse_tool_exited(registry, dispatcher);
      break;

    case ToolType::bucket:
    case ToolType::point:
      [[fallthrough]];
    case ToolType::none:
      break;
  }
}

void on_tool_pressed(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::none:
      break;

    case ToolType::stamp:
      on_stamp_tool_pressed(registry, mouse);
      break;

    case ToolType::bucket:
      on_bucket_tool_pressed(registry, dispatcher, mouse);
      break;

    case ToolType::eraser:
      on_eraser_tool_pressed(registry, mouse);
      break;

    case ToolType::object_selection:
      on_object_selection_tool_pressed(registry, dispatcher, mouse);
      break;

    case ToolType::rectangle:
      on_rectangle_tool_pressed(registry, mouse);
      break;

    case ToolType::point:
      on_point_tool_pressed(registry, dispatcher, mouse);
      break;

    case ToolType::ellipse:
      on_ellipse_tool_pressed(registry, mouse);
      break;
  }
}

void on_tool_dragged(entt::registry& registry,
                     [[maybe_unused]] entt::dispatcher& dispatcher,
                     const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::none:
    case ToolType::point:
      [[fallthrough]];
    case ToolType::bucket:
      break;

    case ToolType::stamp:
      on_stamp_tool_dragged(registry, mouse);
      break;

    case ToolType::eraser:
      on_eraser_tool_dragged(registry, mouse);
      break;

    case ToolType::object_selection:
      on_object_selection_tool_dragged(registry, dispatcher, mouse);
      break;

    case ToolType::rectangle:
      on_rectangle_tool_dragged(registry, mouse);
      break;

    case ToolType::ellipse:
      on_ellipse_tool_dragged(registry, mouse);
      break;
  }
}

void on_tool_released(entt::registry& registry,
                      entt::dispatcher& dispatcher,
                      const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::none:
      [[fallthrough]];
    case ToolType::bucket:
    case ToolType::point:
      break;

    case ToolType::stamp:
      on_stamp_tool_released(registry, dispatcher, mouse);
      break;

    case ToolType::eraser:
      on_eraser_tool_released(registry, dispatcher, mouse);
      break;

    case ToolType::object_selection:
      on_object_selection_tool_released(registry, dispatcher, mouse);
      break;

    case ToolType::rectangle:
      on_rectangle_tool_released(registry, dispatcher, mouse);
      break;

    case ToolType::ellipse:
      on_ellipse_tool_released(registry, dispatcher, mouse);
      break;
  }
}

auto is_tool_enabled(const entt::registry& registry, const ToolType tool) -> bool
{
  const auto& active = registry.ctx<comp::ActiveTool>();
  return active.tool == tool;
}

}  // namespace tactile::sys
