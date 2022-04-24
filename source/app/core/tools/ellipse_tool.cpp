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

#include "ellipse_tool.hpp"

#include <cmath>  // abs

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <glm/vec2.hpp>

#include "core/components/tools.hpp"
#include "core/renderer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/tool_events.hpp"

namespace tactile {

void EllipseTool::draw_gizmos(const entt::registry& registry,
                              IRenderer& renderer,
                              const MouseInfo&) const
{
  if (const auto* stroke = registry.ctx().find<comp::CurrentEllipseStroke>()) {
    const glm::vec2 radius{stroke->current_x - stroke->start_x,
                           stroke->current_y - stroke->start_y};
    const glm::vec2 center{stroke->start_x + radius.x, stroke->start_y + radius.y};

    renderer.draw_ellipse(center + glm::vec2{1, 1}, radius, cen::colors::black);
    renderer.draw_ellipse(center, radius, cen::colors::yellow);
  }
}

void EllipseTool::on_disabled(entt::registry& registry, entt::dispatcher& dispatcher)
{
  maybe_emit_event(registry, dispatcher);
}

void EllipseTool::on_exited(entt::registry& registry, entt::dispatcher& dispatcher)
{
  maybe_emit_event(registry, dispatcher);
}

void EllipseTool::on_pressed(entt::registry& registry,
                             entt::dispatcher&,
                             const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      sys::is_object_layer_active(registry)) {
    auto& stroke = registry.ctx().emplace<comp::CurrentEllipseStroke>();
    stroke.start_x = mouse.x;
    stroke.start_y = mouse.y;
    stroke.current_x = stroke.start_x;
    stroke.current_y = stroke.start_y;
  }
}

void EllipseTool::on_dragged(entt::registry& registry,
                             entt::dispatcher&,
                             const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && sys::is_object_layer_active(registry)) {
    if (auto* stroke = registry.ctx().find<comp::CurrentEllipseStroke>()) {
      stroke->current_x = mouse.x;
      stroke->current_y = mouse.y;
    }
  }
}

void EllipseTool::on_released(entt::registry& registry,
                              entt::dispatcher& dispatcher,
                              const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && sys::is_object_layer_active(registry)) {
    maybe_emit_event(registry, dispatcher);
  }
}

auto EllipseTool::is_available(const entt::registry& registry) const -> bool
{
  return sys::is_object_layer_active(registry);
}

auto EllipseTool::get_type() const -> ToolType
{
  return ToolType::Ellipse;
}

void EllipseTool::maybe_emit_event(entt::registry& registry, entt::dispatcher& dispatcher)
{
  auto& ctx = registry.ctx();
  if (const auto* stroke = ctx.find<comp::CurrentEllipseStroke>()) {
    const auto [xRatio, yRatio] = sys::get_viewport_scaling_ratio(registry);

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

    ctx.erase<comp::CurrentEllipseStroke>();
  }
}

}  // namespace tactile