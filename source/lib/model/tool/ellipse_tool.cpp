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

#include "common/debug/assert.hpp"
#include "common/type/math.hpp"
#include "components/document.hpp"
#include "components/map.hpp"
#include "core/layer.hpp"
#include "core/viewport.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"

namespace tactile {

void EllipseTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void EllipseTool::on_disabled(Model& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EllipseTool::on_exited(Model& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EllipseTool::on_pressed(Model& model, entt::dispatcher&, const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      is_available(model)) {
    auto& stroke = mStroke.emplace();
    stroke.start = mouse.pos;
    stroke.current = stroke.start;
  }
}

void EllipseTool::on_dragged(Model& model, entt::dispatcher&, const MouseInfo& mouse)
{
  if (mStroke && mouse.button == cen::mouse_button::left && is_available(model)) {
    mStroke->current = mouse.pos;
  }
}

void EllipseTool::on_released(Model& model,
                              entt::dispatcher& dispatcher,
                              const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

void EllipseTool::maybe_emit_event(Model& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(is_available(model));

  if (mStroke) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    const auto& viewport = model.get<Viewport>(document_entity);
    const auto scaling_ratio = viewport.tile_size / Float2 {map.tile_size};

    const auto radius = (mStroke->current - mStroke->start) / scaling_ratio;
    auto pos = mStroke->start / scaling_ratio;

    if (radius.x < 0) {
      pos.x += radius.x * 2.0f;
    }

    if (radius.y < 0) {
      pos.y += radius.y * 2.0f;
    }

    if (radius.x != 0 && radius.y != 0) {
      const auto diameter = glm::abs(radius) * 2.0f;
      dispatcher.enqueue<AddEllipseEvent>(map.active_layer, pos, diameter);
    }

    mStroke.reset();
  }
}

auto EllipseTool::is_available(const Model& model) const -> bool
{
  const auto document_entity = sys::get_active_document(model);
  if (document_entity == kNullEntity || !model.has<MapDocument>(document_entity)) {
    return false;
  }

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);
  if (map.active_layer != kNullEntity && model.has<ObjectLayer>(map.active_layer)) {
    return true;
  }

  return false;
}

auto EllipseTool::get_stroke() const -> const Maybe<CurrentEllipseStroke>&
{
  return mStroke;
}

}  // namespace tactile