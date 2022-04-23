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

#include "rectangle_tool.hpp"

#include <algorithm>  // min
#include <cmath>      // abs

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <glm/vec2.hpp>

#include "core/components/tools.hpp"
#include "core/renderer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile {

void RectangleTool::draw_gizmos(const entt::registry& registry,
                                IRenderer& renderer,
                                const MouseInfo&) const
{
  if (const auto* stroke = registry.ctx().find<comp::CurrentRectangleStroke>()) {
    const auto pos = renderer.get_origin() + glm::vec2{stroke->start_x, stroke->start_y};
    const glm::vec2 size{stroke->current_x - stroke->start_x,
                         stroke->current_y - stroke->start_y};

    renderer.draw_rect(pos + glm::vec2{1, 1}, size, cen::colors::black);
    renderer.draw_rect(pos, size, cen::colors::yellow);
  }
}

void RectangleTool::on_disabled(entt::registry& registry, entt::dispatcher& dispatcher)
{
  maybe_emit_event(registry, dispatcher);
}

void RectangleTool::on_exited(entt::registry& registry, entt::dispatcher& dispatcher)
{
  maybe_emit_event(registry, dispatcher);
}

void RectangleTool::on_pressed(entt::registry& registry,
                               entt::dispatcher&,
                               const MouseInfo& mouse)
{
  if (sys::is_object_layer_active(registry) && mouse.is_within_contents &&
      mouse.button == cen::mouse_button::left) {
    auto& stroke = registry.ctx().emplace<comp::CurrentRectangleStroke>();
    stroke.start_x = mouse.x;
    stroke.start_y = mouse.y;
    stroke.current_x = stroke.start_x;
    stroke.current_y = stroke.start_y;
  }
}

void RectangleTool::on_dragged(entt::registry& registry,
                               entt::dispatcher&,
                               const MouseInfo& mouse)
{
  if (sys::is_object_layer_active(registry) && mouse.button == cen::mouse_button::left) {
    if (auto* stroke = registry.ctx().find<comp::CurrentRectangleStroke>()) {
      stroke->current_x = mouse.x;
      stroke->current_y = mouse.y;
    }
  }
}

void RectangleTool::on_released(entt::registry& registry,
                                entt::dispatcher& dispatcher,
                                const MouseInfo& mouse)
{
  if (sys::is_object_layer_active(registry) && mouse.button == cen::mouse_button::left) {
    maybe_emit_event(registry, dispatcher);
  }
}

auto RectangleTool::is_available(const entt::registry& registry) const -> bool
{
  return sys::is_object_layer_active(registry);
}

auto RectangleTool::get_type() const -> ToolType
{
  return ToolType::Rectangle;
}

void RectangleTool::maybe_emit_event(entt::registry& registry,
                                     entt::dispatcher& dispatcher)
{
  auto& ctx = registry.ctx();
  if (const auto* stroke = ctx.find<comp::CurrentRectangleStroke>()) {
    const auto [xRatio, yRatio] = sys::get_viewport_scaling_ratio(registry);

    const auto x = (std::min)(stroke->start_x, stroke->current_x) / xRatio;
    const auto y = (std::min)(stroke->start_y, stroke->current_y) / yRatio;
    const auto width = std::abs(stroke->current_x - stroke->start_x) / xRatio;
    const auto height = std::abs(stroke->current_y - stroke->start_y) / yRatio;

    if (width != 0 && height != 0) {
      dispatcher.enqueue<AddRectangleEvent>(x, y, width, height);
    }

    ctx.erase<comp::CurrentRectangleStroke>();
  }
}

}  // namespace tactile