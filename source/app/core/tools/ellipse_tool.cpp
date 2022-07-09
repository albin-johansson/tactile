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

#include <entt/signal/dispatcher.hpp>
#include <glm/common.hpp>

#include "core/common/math.hpp"
#include "core/documents/map_document.hpp"
#include "core/events/tool_events.hpp"
#include "core/model.hpp"
#include "core/renderer.hpp"
#include "misc/assert.hpp"

namespace tactile {

void EllipseTool::draw_gizmos(const DocumentModel&,
                              IRenderer& renderer,
                              const MouseInfo&) const
{
  if (mStroke) {
    const auto radius = mStroke->current - mStroke->start;
    const auto center = mStroke->start + radius;

    renderer.draw_ellipse(center + Vector2f {1, 1}, radius, cen::colors::black);
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
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      is_available(model)) {
    auto& stroke = mStroke.emplace();
    stroke.start = mouse.pos;
    stroke.current = stroke.start;
  }
}

void EllipseTool::on_dragged(DocumentModel& model,
                             entt::dispatcher&,
                             const MouseInfo& mouse)
{
  if (mStroke && mouse.button == cen::mouse_button::left && is_available(model)) {
    mStroke->current = mouse.pos;
  }
}

void EllipseTool::on_released(DocumentModel&    model,
                              entt::dispatcher& dispatcher,
                              const MouseInfo&  mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

void EllipseTool::maybe_emit_event(DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(is_available(model));

  if (mStroke) {
    const auto& document = model.require_active_map();
    const auto& map = document.get_map();
    const auto& viewport = document.get_viewport();

    const auto ratio = viewport.get_scaling_ratio(map.tile_size());

    const auto radius = (mStroke->current - mStroke->start) / ratio;
    auto       pos = mStroke->start / ratio;

    if (radius.x < 0) {
      pos.x += radius.x * 2.0f;
    }

    if (radius.y < 0) {
      pos.y += radius.y * 2.0f;
    }

    if (radius.x != 0 && radius.y != 0) {
      const auto layerId = map.active_layer_id().value();
      const auto diameter = glm::abs(radius) * 2.0f;
      dispatcher.enqueue<AddEllipseEvent>(layerId, pos, diameter);
    }

    mStroke.reset();
  }
}

auto EllipseTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();
  return map.is_active_layer(LayerType::ObjectLayer);
}

}  // namespace tactile