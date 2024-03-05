/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/type/math.hpp"
#include "model/document/map_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "tactile/core/debug/assert.hpp"

namespace tactile {

void EllipseTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
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

void EllipseTool::on_released(DocumentModel& model,
                              entt::dispatcher& dispatcher,
                              const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

void EllipseTool::maybe_emit_event(DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(is_available(model));

  if (mStroke) {
    const auto& map_document = model.require_active_map_document();
    const auto& map = map_document.get_map();
    const auto& viewport = map_document.get_viewport();

    const auto ratio = viewport.scaling_ratio(map.get_tile_size());

    const auto radius = (mStroke->current - mStroke->start) / ratio;
    auto pos = mStroke->start / ratio;

    if (radius.x < 0) {
      pos.x += radius.x * 2.0f;
    }

    if (radius.y < 0) {
      pos.y += radius.y * 2.0f;
    }

    if (radius.x != 0 && radius.y != 0) {
      const auto layer_id = map.get_active_layer_id().value();
      const auto diameter = glm::abs(radius) * 2.0f;
      dispatcher.enqueue<AddEllipseEvent>(layer_id, pos, diameter);
    }

    mStroke.reset();
  }
}

auto EllipseTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& map_document = model.require_active_map_document();
  const auto& map = map_document.get_map();
  return map.is_active_layer(LayerType::ObjectLayer);
}

auto EllipseTool::get_stroke() const -> const Maybe<CurrentEllipseStroke>&
{
  return mStroke;
}

}  // namespace tactile
