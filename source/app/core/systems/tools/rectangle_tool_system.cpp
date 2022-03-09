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

#include "rectangle_tool_system.hpp"

#include <algorithm>  // min
#include <cmath>      // abs

#include "core/components/tool.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _is_usable(const entt::registry& registry) -> bool
{
  return is_object_layer_active(registry);
}

void _maybe_emit_event(entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (const auto* stroke = registry.try_ctx<comp::CurrentRectangleStroke>()) {
    const auto [xRatio, yRatio] = GetViewportScalingRatio(registry);

    const auto x = (std::min)(stroke->start_x, stroke->current_x) / xRatio;
    const auto y = (std::min)(stroke->start_y, stroke->current_y) / yRatio;
    const auto width = std::abs(stroke->current_x - stroke->start_x) / xRatio;
    const auto height = std::abs(stroke->current_y - stroke->start_y) / yRatio;

    if (width != 0 && height != 0) {
      dispatcher.enqueue<AddRectangleEvent>(x, y, width, height);
    }

    registry.unset<comp::CurrentRectangleStroke>();
  }
}

}  // namespace

void on_rectangle_tool_disabled(entt::registry& registry, entt::dispatcher& dispatcher)
{
  _maybe_emit_event(registry, dispatcher);
}

void on_rectangle_tool_exited(entt::registry& registry, entt::dispatcher& dispatcher)
{
  _maybe_emit_event(registry, dispatcher);
}

void on_rectangle_tool_pressed(entt::registry& registry, const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.is_within_contents &&
      mouse.button == cen::mouse_button::left) {
    auto& stroke = registry.set<comp::CurrentRectangleStroke>();
    stroke.start_x = mouse.x;
    stroke.start_y = mouse.y;
    stroke.current_x = stroke.start_x;
    stroke.current_y = stroke.start_y;
  }
}

void on_rectangle_tool_dragged(entt::registry& registry, const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.button == cen::mouse_button::left) {
    if (auto* stroke = registry.try_ctx<comp::CurrentRectangleStroke>()) {
      stroke->current_x = mouse.x;
      stroke->current_y = mouse.y;
    }
  }
}

void on_rectangle_tool_released(entt::registry& registry,
                                entt::dispatcher& dispatcher,
                                const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.button == cen::mouse_button::left) {
    _maybe_emit_event(registry, dispatcher);
  }
}

}  // namespace tactile::sys