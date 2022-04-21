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

#include <entt/entity/registry.hpp>

#include "core/components/tools.hpp"
#include "core/systems/tools/bucket_tool_system.hpp"
#include "core/systems/tools/ellipse_tool_system.hpp"
#include "core/systems/tools/eraser_tool_system.hpp"
#include "core/systems/tools/object_selection_tool_system.hpp"
#include "core/systems/tools/point_tool_system.hpp"
#include "core/systems/tools/rectangle_tool_system.hpp"
#include "core/systems/tools/stamp_tool_system.hpp"

namespace tactile::sys {

void select_tool(entt::registry& registry,
                 entt::dispatcher& dispatcher,
                 const ToolType tool)
{
  auto& active = registry.ctx().at<comp::ActiveTool>();

  switch (active.tool) {
    case ToolType::Stamp:
      on_stamp_tool_disabled(dispatcher);
      break;

    case ToolType::Eraser:
      on_eraser_tool_disabled(dispatcher);
      break;

    case ToolType::Rectangle:
      on_rectangle_tool_disabled(registry, dispatcher);
      break;

    case ToolType::Ellipse:
      on_ellipse_tool_disabled(registry, dispatcher);
      break;

    case ToolType::Bucket:
    case ToolType::ObjectSelection:
    case ToolType::Point:
      [[fallthrough]];
    case ToolType::None:
      break;
  }

  active.tool = (active.tool == tool) ? ToolType::None : tool;
}

void on_tool_entered(entt::registry& registry, entt::dispatcher&)
{
  const auto& active = registry.ctx().at<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::Stamp:
    case ToolType::Eraser:
    case ToolType::Bucket:
    case ToolType::ObjectSelection:
    case ToolType::Rectangle:
    case ToolType::Ellipse:
    case ToolType::Point:
      [[fallthrough]];
    case ToolType::None:
      break;
  }
}

void on_tool_exited(entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto& active = registry.ctx().at<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::Stamp:
      on_stamp_tool_exited(dispatcher);
      break;

    case ToolType::Eraser:
      on_eraser_tool_exited(dispatcher);
      break;

    case ToolType::ObjectSelection:
      on_object_selection_tool_exited(registry, dispatcher);
      break;

    case ToolType::Rectangle:
      on_rectangle_tool_exited(registry, dispatcher);
      break;

    case ToolType::Ellipse:
      on_ellipse_tool_exited(registry, dispatcher);
      break;

    case ToolType::Bucket:
    case ToolType::Point:
      [[fallthrough]];
    case ToolType::None:
      break;
  }
}

void on_tool_pressed(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const MouseInfo& mouse)
{
  const auto& active = registry.ctx().at<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::None:
      break;

    case ToolType::Stamp:
      on_stamp_tool_pressed(registry, mouse);
      break;

    case ToolType::Bucket:
      on_bucket_tool_pressed(registry, dispatcher, mouse);
      break;

    case ToolType::Eraser:
      on_eraser_tool_pressed(registry, mouse);
      break;

    case ToolType::ObjectSelection:
      on_object_selection_tool_pressed(registry, dispatcher, mouse);
      break;

    case ToolType::Rectangle:
      on_rectangle_tool_pressed(registry, mouse);
      break;

    case ToolType::Point:
      on_point_tool_pressed(registry, dispatcher, mouse);
      break;

    case ToolType::Ellipse:
      on_ellipse_tool_pressed(registry, mouse);
      break;
  }
}

void on_tool_dragged(entt::registry& registry,
                     [[maybe_unused]] entt::dispatcher& dispatcher,
                     const MouseInfo& mouse)
{
  const auto& active = registry.ctx().at<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::None:
    case ToolType::Point:
      [[fallthrough]];
    case ToolType::Bucket:
      break;

    case ToolType::Stamp:
      on_stamp_tool_dragged(registry, mouse);
      break;

    case ToolType::Eraser:
      on_eraser_tool_dragged(registry, mouse);
      break;

    case ToolType::ObjectSelection:
      on_object_selection_tool_dragged(registry, dispatcher, mouse);
      break;

    case ToolType::Rectangle:
      on_rectangle_tool_dragged(registry, mouse);
      break;

    case ToolType::Ellipse:
      on_ellipse_tool_dragged(registry, mouse);
      break;
  }
}

void on_tool_released(entt::registry& registry,
                      entt::dispatcher& dispatcher,
                      const MouseInfo& mouse)
{
  const auto& active = registry.ctx().at<comp::ActiveTool>();
  switch (active.tool) {
    case ToolType::None:
      [[fallthrough]];
    case ToolType::Bucket:
    case ToolType::Point:
      break;

    case ToolType::Stamp:
      on_stamp_tool_released(registry, dispatcher, mouse);
      break;

    case ToolType::Eraser:
      on_eraser_tool_released(registry, dispatcher, mouse);
      break;

    case ToolType::ObjectSelection:
      on_object_selection_tool_released(registry, dispatcher, mouse);
      break;

    case ToolType::Rectangle:
      on_rectangle_tool_released(registry, dispatcher, mouse);
      break;

    case ToolType::Ellipse:
      on_ellipse_tool_released(registry, dispatcher, mouse);
      break;
  }
}

auto is_tool_enabled(const entt::registry& registry, const ToolType tool) -> bool
{
  const auto& active = registry.ctx().at<comp::ActiveTool>();
  return active.tool == tool;
}

}  // namespace tactile::sys
