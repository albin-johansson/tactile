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

#include "ellipse_tool_system.hpp"

#include <cmath>  // abs

#include "core/components/tools.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile::sys {
namespace {

void _maybe_emit_event(entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (const auto* stroke = registry.try_ctx<comp::CurrentEllipseStroke>()) {
    const auto [xRatio, yRatio] = GetViewportScalingRatio(registry);

    const auto xRadius = (stroke->current_x - stroke->start_x) / xRatio;
    const auto yRadius = (stroke->current_y - stroke->start_y) / yRatio;

    auto x = (stroke->start_x / xRatio);
    auto y = (stroke->start_y / yRatio);

    if (xRadius < 0) {
      x += xRadius * 2.0f;
    }

    if (yRadius < 0) {
      y += yRadius * 2.0f;
    }

    if (xRadius != 0 && yRadius != 0) {
      dispatcher.enqueue<AddEllipseEvent>(x,
                                          y,
                                          std::abs(xRadius) * 2.0f,
                                          std::abs(yRadius) * 2.0f);
    }

    registry.unset<comp::CurrentEllipseStroke>();
  }
}

}  // namespace

void on_ellipse_tool_disabled(entt::registry& registry, entt::dispatcher& dispatcher)
{
  _maybe_emit_event(registry, dispatcher);
}

void on_ellipse_tool_exited(entt::registry& registry, entt::dispatcher& dispatcher)
{
  _maybe_emit_event(registry, dispatcher);
}

void on_ellipse_tool_pressed(entt::registry& registry, const MouseInfo& mouse)
{
  if (is_object_layer_active(registry) && mouse.is_within_contents &&
      mouse.button == cen::mouse_button::left) {
    auto& stroke = registry.set<comp::CurrentEllipseStroke>();
    stroke.start_x = mouse.x;
    stroke.start_y = mouse.y;
    stroke.current_x = stroke.start_x;
    stroke.current_y = stroke.start_y;
  }
}

void on_ellipse_tool_dragged(entt::registry& registry, const MouseInfo& mouse)
{
  if (is_object_layer_active(registry) && mouse.button == cen::mouse_button::left) {
    if (auto* stroke = registry.try_ctx<comp::CurrentEllipseStroke>()) {
      stroke->current_x = mouse.x;
      stroke->current_y = mouse.y;
    }
  }
}

void on_ellipse_tool_released(entt::registry& registry,
                              entt::dispatcher& dispatcher,
                              const MouseInfo& mouse)
{
  if (is_object_layer_active(registry) && mouse.button == cen::mouse_button::left) {
    _maybe_emit_event(registry, dispatcher);
  }
}

}  // namespace tactile::sys