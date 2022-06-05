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

#include "object_selection_tool.hpp"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/components/objects.hpp"
#include "core/components/tools.hpp"
#include "core/events/object_events.hpp"
#include "core/events/tool_events.hpp"
#include "core/model.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "core/systems/viewport_system.hpp"

namespace tactile {

void ObjectSelectionTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void ObjectSelectionTool::on_pressed(DocumentModel& model,
                                     entt::dispatcher& dispatcher,
                                     const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (sys::is_object_layer_active(registry)) {
    auto& active = ctx_get<comp::ActiveObject>(registry);

    const auto& [_, layer] =
        sys::get_object_layer(registry, sys::get_active_layer_id(registry).value());
    const auto objectEntity = sys::find_object(registry, layer, mouse.x, mouse.y);

    switch (mouse.button) {
      case cen::mouse_button::left: {
        active.entity = objectEntity;

        if (objectEntity != entt::null) {
          const auto& object = checked_get<comp::Object>(registry, objectEntity);

          auto& drag = registry.emplace<comp::ObjectDragInfo>(objectEntity);
          drag.origin_object_x = object.x;
          drag.origin_object_y = object.y;
          drag.last_mouse_x = mouse.x;
          drag.last_mouse_y = mouse.y;
        }

        break;
      }
      case cen::mouse_button::right: {
        active.entity = objectEntity;

        if (objectEntity != entt::null) {
          dispatcher.enqueue<SpawnObjectContextMenuEvent>(objectEntity);
        }

        break;
      }
      default:
        break;
    }
  }
}

void ObjectSelectionTool::on_dragged(DocumentModel& model,
                                     entt::dispatcher& dispatcher,
                                     const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (mouse.button == cen::mouse_button::left && sys::is_object_layer_active(registry)) {
    const auto& active = ctx_get<comp::ActiveObject>(registry);
    if (active.entity != entt::null) {
      if (auto* drag = registry.try_get<comp::ObjectDragInfo>(active.entity)) {
        auto& object = checked_get<comp::Object>(registry, active.entity);
        if (mouse.is_within_contents) {
          const auto [xRatio, yRatio] = sys::get_viewport_scaling_ratio(registry);
          const auto dx = (mouse.x - drag->last_mouse_x) / xRatio;
          const auto dy = (mouse.y - drag->last_mouse_y) / yRatio;

          object.x += dx;
          object.y += dy;

          drag->last_mouse_x = mouse.x;
          drag->last_mouse_y = mouse.y;
        }
        else {
          /* Stop if the user drags the object outside the map */
          maybe_emit_event(model, dispatcher);
        }
      }
    }
  }
}

void ObjectSelectionTool::on_released(DocumentModel& model,
                                      entt::dispatcher& dispatcher,
                                      const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (mouse.button == cen::mouse_button::left && sys::is_object_layer_active(registry)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto ObjectSelectionTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& registry = model.get_active_registry();
  return sys::is_object_layer_active(registry);
}

auto ObjectSelectionTool::get_type() const -> ToolType
{
  return ToolType::ObjectSelection;
}

void ObjectSelectionTool::maybe_emit_event(DocumentModel& model,
                                           entt::dispatcher& dispatcher)
{
  auto& registry = model.get_active_registry();
  const auto entity = ctx_get<comp::ActiveObject>(registry).entity;
  if (entity != entt::null) {
    if (const auto* drag = registry.try_get<comp::ObjectDragInfo>(entity)) {
      const auto& object = checked_get<comp::Object>(registry, entity);

      /* Only emit an event if the object has been moved along any axis */
      if (drag->origin_object_x != object.x || drag->origin_object_y != object.y) {
        const auto& context = checked_get<comp::Context>(registry, entity);
        dispatcher.enqueue<MoveObjectEvent>(
            context.id,
            Vector2f{drag->origin_object_x, drag->origin_object_y},
            Vector2f{object.x, object.y});
      }

      registry.remove<comp::ObjectDragInfo>(entity);
    }
  }
}

}  // namespace tactile