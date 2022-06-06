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

#include <entt/signal/dispatcher.hpp>
#include <glm/common.hpp>

#include "core/common/math.hpp"
#include "core/components/tools.hpp"
#include "core/events/tool_events.hpp"
#include "core/fwd.hpp"
#include "core/model.hpp"
#include "core/renderer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/viewport_system.hpp"

namespace tactile {

void RectangleTool::draw_gizmos(const DocumentModel& model,
                                IRenderer& renderer,
                                const MouseInfo&) const
{
  const auto& registry = model.get_active_registry();
  if (const auto* stroke = registry.ctx().find<comp::CurrentRectangleStroke>()) {
    const auto pos = renderer.get_origin() + stroke->start;
    const auto size = stroke->current - stroke->start;

    renderer.draw_rect(pos + Vector2f{1, 1}, size, cen::colors::black);
    renderer.draw_rect(pos, size, cen::colors::yellow);
  }
}

void RectangleTool::on_disabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void RectangleTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void RectangleTool::on_pressed(DocumentModel& model,
                               entt::dispatcher&,
                               const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (sys::is_object_layer_active(registry) && mouse.is_within_contents &&
      mouse.button == cen::mouse_button::left) {
    auto& stroke = registry.ctx().emplace<comp::CurrentRectangleStroke>();
    stroke.start = mouse.pos;
    stroke.current = stroke.start;
  }
}

void RectangleTool::on_dragged(DocumentModel& model,
                               entt::dispatcher&,
                               const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (sys::is_object_layer_active(registry) && mouse.button == cen::mouse_button::left) {
    if (auto* stroke = registry.ctx().find<comp::CurrentRectangleStroke>()) {
      stroke->current = mouse.pos;
    }
  }
}

void RectangleTool::on_released(DocumentModel& model,
                                entt::dispatcher& dispatcher,
                                const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (sys::is_object_layer_active(registry) && mouse.button == cen::mouse_button::left) {
    maybe_emit_event(model, dispatcher);
  }
}

auto RectangleTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& registry = model.get_active_registry();
  return sys::is_object_layer_active(registry);
}

auto RectangleTool::get_type() const -> ToolType
{
  return ToolType::Rectangle;
}

void RectangleTool::maybe_emit_event(DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& registry = model.get_active_registry();
  auto& ctx = registry.ctx();
  if (const auto* stroke = ctx.find<comp::CurrentRectangleStroke>()) {
    const auto ratio = sys::get_viewport_scaling_ratio(registry);

    const auto pos = (glm::min)(stroke->start, stroke->current) / ratio;
    const auto size = glm::abs(stroke->current - stroke->start) / ratio;

    if (size.x != 0 && size.y != 0) {
      dispatcher.enqueue<AddRectangleEvent>(pos, size);
    }

    ctx.erase<comp::CurrentRectangleStroke>();
  }
}

}  // namespace tactile