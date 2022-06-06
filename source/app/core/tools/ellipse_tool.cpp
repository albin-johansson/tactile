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
#include <glm/common.hpp>

#include "core/common/math.hpp"
#include "core/components/tools.hpp"
#include "core/events/tool_events.hpp"
#include "core/model.hpp"
#include "core/renderer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/viewport_system.hpp"

namespace tactile {

void EllipseTool::draw_gizmos(const DocumentModel& model,
                              IRenderer& renderer,
                              const MouseInfo&) const
{
  const auto& registry = model.get_active_registry();
  if (const auto* stroke = registry.ctx().find<comp::CurrentEllipseStroke>()) {
    const auto radius = stroke->current - stroke->start;
    const auto center = stroke->start + radius;

    renderer.draw_ellipse(center + Vector2f{1, 1}, radius, cen::colors::black);
    renderer.draw_ellipse(center, radius, cen::colors::yellow);
  }
}

void EllipseTool::on_disabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EllipseTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EllipseTool::on_pressed(DocumentModel& model,
                             entt::dispatcher&,
                             const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      sys::is_object_layer_active(registry)) {
    auto& stroke = registry.ctx().emplace<comp::CurrentEllipseStroke>();
    stroke.start = mouse.pos;
    stroke.current = stroke.start;
  }
}

void EllipseTool::on_dragged(DocumentModel& model,
                             entt::dispatcher&,
                             const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (mouse.button == cen::mouse_button::left && sys::is_object_layer_active(registry)) {
    if (auto* stroke = registry.ctx().find<comp::CurrentEllipseStroke>()) {
      stroke->current = mouse.pos;
    }
  }
}

void EllipseTool::on_released(DocumentModel& model,
                              entt::dispatcher& dispatcher,
                              const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (mouse.button == cen::mouse_button::left && sys::is_object_layer_active(registry)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto EllipseTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& registry = model.get_active_registry();
  return sys::is_object_layer_active(registry);
}

auto EllipseTool::get_type() const -> ToolType
{
  return ToolType::Ellipse;
}

void EllipseTool::maybe_emit_event(DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& registry = model.get_active_registry();
  auto& ctx = registry.ctx();
  if (const auto* stroke = ctx.find<comp::CurrentEllipseStroke>()) {
    const auto ratio = sys::get_viewport_scaling_ratio(registry);

    const auto radius = (stroke->current - stroke->start) / ratio;
    auto pos = stroke->start / ratio;

    if (radius.x < 0) {
      pos.x += radius.x * 2.0f;
    }

    if (radius.y < 0) {
      pos.y += radius.y * 2.0f;
    }

    if (radius.x != 0 && radius.y != 0) {
      const auto diameter = glm::abs(radius) * 2.0f;
      dispatcher.enqueue<AddEllipseEvent>(pos, diameter);
    }

    ctx.erase<comp::CurrentEllipseStroke>();
  }
}

}  // namespace tactile