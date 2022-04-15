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

#include "object_selection_tool_system.hpp"

#include "core/components/objects.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/object_events.hpp"

namespace tactile::sys {
namespace {

void _maybe_emit_event(entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto entity = registry.ctx<comp::ActiveObject>().entity;
  if (entity != entt::null) {
    if (const auto* drag = registry.try_get<comp::ObjectDragInfo>(entity)) {
      const auto& object = checked_get<comp::Object>(registry, entity);

      /* Only emit an event if the object has been moved along any axis */
      if (drag->origin_object_x != object.x || drag->origin_object_y != object.y) {
        dispatcher.enqueue<MoveObjectEvent>(object.id,
                                            drag->origin_object_x,
                                            drag->origin_object_y,
                                            object.x,
                                            object.y);
      }

      registry.remove<comp::ObjectDragInfo>(entity);
    }
  }
}

[[nodiscard]] auto _find_object_at_mouse(entt::registry& registry, const MouseInfo& mouse)
    -> entt::entity
{
  const auto& [_, layer] =
      sys::get_object_layer(registry, get_active_layer_id(registry).value());
  return find_object(registry, layer, mouse.x, mouse.y);
}

}  // namespace

void on_object_selection_tool_exited(entt::registry& registry,
                                     entt::dispatcher& dispatcher)
{
  _maybe_emit_event(registry, dispatcher);
}

void on_object_selection_tool_pressed(entt::registry& registry,
                                      entt::dispatcher& dispatcher,
                                      const MouseInfo& mouse)
{
  if (is_object_layer_active(registry)) {
    auto& active = registry.ctx<comp::ActiveObject>();
    const auto objectEntity = _find_object_at_mouse(registry, mouse);
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

void on_object_selection_tool_dragged(entt::registry& registry,
                                      entt::dispatcher& dispatcher,
                                      const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_object_layer_active(registry)) {
    const auto& active = registry.ctx<comp::ActiveObject>();
    if (active.entity != entt::null) {
      if (auto* drag = registry.try_get<comp::ObjectDragInfo>(active.entity)) {
        auto& object = checked_get<comp::Object>(registry, active.entity);
        if (mouse.is_within_contents) {
          const auto [xRatio, yRatio] = get_viewport_scaling_ratio(registry);
          const auto dx = (mouse.x - drag->last_mouse_x) / xRatio;
          const auto dy = (mouse.y - drag->last_mouse_y) / yRatio;

          object.x += dx;
          object.y += dy;

          drag->last_mouse_x = mouse.x;
          drag->last_mouse_y = mouse.y;
        }
        else {
          /* Stop if the user drags the object outside the map */
          _maybe_emit_event(registry, dispatcher);
        }
      }
    }
  }
}

void on_object_selection_tool_released(entt::registry& registry,
                                       entt::dispatcher& dispatcher,
                                       const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_object_layer_active(registry)) {
    _maybe_emit_event(registry, dispatcher);
  }
}

}  // namespace tactile::sys
