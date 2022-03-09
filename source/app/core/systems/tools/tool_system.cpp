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
#include "core/components/tool.hpp"
#include "ellipse_tool_system.hpp"
#include "eraser_tool_system.hpp"
#include "object_selection_tool_system.hpp"
#include "point_tool_system.hpp"
#include "rectangle_tool_system.hpp"
#include "stamp_tool_system.hpp"

namespace tactile::sys {

void select_tool(entt::registry& registry,
                 entt::dispatcher& dispatcher,
                 const tool_type tool)
{
  auto& active = registry.ctx<comp::active_tool>();

  switch (active.tool) {
    case tool_type::stamp:
      on_stamp_tool_disabled(dispatcher);
      break;

    case tool_type::eraser:
      on_eraser_tool_disabled(dispatcher);
      break;

    case tool_type::rectangle:
      on_rectangle_tool_disabled(registry, dispatcher);
      break;

    case tool_type::ellipse:
      on_ellipse_tool_disabled(registry, dispatcher);
      break;

    case tool_type::bucket:
    case tool_type::object_selection:
    case tool_type::point:
      [[fallthrough]];
    case tool_type::none:
      break;
  }

  active.tool = (active.tool == tool) ? tool_type::none : tool;
}

void on_tool_entered(entt::registry& registry, entt::dispatcher&)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::stamp:
    case tool_type::eraser:
    case tool_type::bucket:
    case tool_type::object_selection:
    case tool_type::rectangle:
    case tool_type::ellipse:
    case tool_type::point:
      [[fallthrough]];
    case tool_type::none:
      break;
  }
}

void on_tool_exited(entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::stamp:
      on_stamp_tool_exited(dispatcher);
      break;

    case tool_type::eraser:
      on_eraser_tool_exited(dispatcher);
      break;

    case tool_type::object_selection:
      on_object_selection_tool_exited(registry, dispatcher);
      break;

    case tool_type::rectangle:
      on_rectangle_tool_exited(registry, dispatcher);
      break;

    case tool_type::ellipse:
      on_ellipse_tool_exited(registry, dispatcher);
      break;

    case tool_type::bucket:
    case tool_type::point:
      [[fallthrough]];
    case tool_type::none:
      break;
  }
}

void on_tool_pressed(entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
      break;

    case tool_type::stamp:
      on_stamp_tool_pressed(registry, mouse);
      break;

    case tool_type::bucket:
      on_bucket_tool_pressed(registry, dispatcher, mouse);
      break;

    case tool_type::eraser:
      on_eraser_tool_pressed(registry, mouse);
      break;

    case tool_type::object_selection:
      on_object_selection_tool_pressed(registry, dispatcher, mouse);
      break;

    case tool_type::rectangle:
      on_rectangle_tool_pressed(registry, mouse);
      break;

    case tool_type::point:
      on_point_tool_pressed(registry, dispatcher, mouse);
      break;

    case tool_type::ellipse:
      on_ellipse_tool_pressed(registry, mouse);
      break;
  }
}

void on_tool_dragged(entt::registry& registry,
                     [[maybe_unused]] entt::dispatcher& dispatcher,
                     const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
    case tool_type::point:
      [[fallthrough]];
    case tool_type::bucket:
      break;

    case tool_type::stamp:
      on_stamp_tool_dragged(registry, mouse);
      break;

    case tool_type::eraser:
      on_eraser_tool_dragged(registry, mouse);
      break;

    case tool_type::object_selection:
      on_object_selection_tool_dragged(registry, dispatcher, mouse);
      break;

    case tool_type::rectangle:
      on_rectangle_tool_dragged(registry, mouse);
      break;

    case tool_type::ellipse:
      on_ellipse_tool_dragged(registry, mouse);
      break;
  }
}

void on_tool_released(entt::registry& registry,
                      entt::dispatcher& dispatcher,
                      const mouse_info& mouse)
{
  const auto& active = registry.ctx<comp::active_tool>();
  switch (active.tool) {
    case tool_type::none:
      [[fallthrough]];
    case tool_type::bucket:
    case tool_type::point:
      break;

    case tool_type::stamp:
      on_stamp_tool_released(registry, dispatcher, mouse);
      break;

    case tool_type::eraser:
      on_eraser_tool_released(registry, dispatcher, mouse);
      break;

    case tool_type::object_selection:
      on_object_selection_tool_released(registry, dispatcher, mouse);
      break;

    case tool_type::rectangle:
      on_rectangle_tool_released(registry, dispatcher, mouse);
      break;

    case tool_type::ellipse:
      on_ellipse_tool_released(registry, dispatcher, mouse);
      break;
  }
}

auto is_tool_enabled(const entt::registry& registry, const tool_type tool) -> bool
{
  const auto& active = registry.ctx<comp::active_tool>();
  return active.tool == tool;
}

}  // namespace tactile::sys
