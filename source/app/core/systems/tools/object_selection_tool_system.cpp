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

#include "core/components/object.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"

namespace tactile::sys {
namespace {

void _emit_event(entt::dispatcher& dispatcher,
                 const comp::object_drag_info& drag,
                 const comp::object& object)
{
  /* Only emit an event if the object has been moved along any axis */
  if (drag.origin_object_x != object.x || drag.origin_object_y != object.y) {
    dispatcher.enqueue<move_object_event>(object.id,
                                          drag.origin_object_x,
                                          drag.origin_object_y,
                                          object.x,
                                          object.y);
  }
}

}  // namespace

void object_selection_tool_on_pressed(entt::registry& registry,
                                      entt::dispatcher& dispatcher,
                                      const mouse_info& mouse)
{
  if (is_object_layer_active(registry)) {
    if (mouse.button == cen::mouse_button::left) {
      auto& active = registry.ctx<comp::active_object>();
      active.entity = entt::null;

      const auto layerEntity = get_active_layer(registry);
      const auto& layer = registry.get<comp::object_layer>(layerEntity);

      const auto objectEntity = find_object(registry, layer, mouse.x, mouse.y);
      if (objectEntity != entt::null) {
        const auto& object = registry.get<comp::object>(objectEntity);

        auto& drag = registry.emplace<comp::object_drag_info>(objectEntity);
        drag.origin_object_x = object.x;
        drag.origin_object_y = object.y;
        drag.last_mouse_x = mouse.x;
        drag.last_mouse_y = mouse.y;

        active.entity = objectEntity;
        dispatcher.enqueue<inspect_context_event>(objectEntity);
      }
    }
    else if (mouse.button == cen::mouse_button::right) {
      auto& active = registry.ctx<comp::active_object>();
      active.entity = entt::null;

      const auto layerEntity = get_active_layer(registry);
      const auto& layer = registry.get<comp::object_layer>(layerEntity);

      const auto objectEntity = find_object(registry, layer, mouse.x, mouse.y);
      if (objectEntity != entt::null) {
        active.entity = objectEntity;
        dispatcher.enqueue<spawn_object_context_menu_event>(objectEntity);
      }
    }
  }
}

void object_selection_tool_on_dragged(entt::registry& registry,
                                      entt::dispatcher& dispatcher,
                                      const mouse_info& mouse)
{
  if (mouse.button == cen::mouse_button::left) {
    const auto entity = registry.ctx<comp::active_object>().entity;
    if (entity != entt::null) {
      if (auto* drag = registry.try_get<comp::object_drag_info>(entity)) {
        auto& object = registry.get<comp::object>(entity);
        if (mouse.is_within_contents) {
          const auto [xRatio, yRatio] = GetViewportScalingRatio(registry);
          const auto dx = (mouse.x - drag->last_mouse_x) / xRatio;
          const auto dy = (mouse.y - drag->last_mouse_y) / yRatio;

          object.x += dx;
          object.y += dy;

          drag->last_mouse_x = mouse.x;
          drag->last_mouse_y = mouse.y;
        }
        else {
          /* Stop if the user drags the object outside the map */
          _emit_event(dispatcher, *drag, object);
          registry.remove<comp::object_drag_info>(entity);
        }
      }
    }
  }
}

void object_selection_tool_on_released(entt::registry& registry,
                                       entt::dispatcher& dispatcher,
                                       const mouse_info& mouse)
{
  if (mouse.button == cen::mouse_button::left) {
    const auto entity = registry.ctx<comp::active_object>().entity;
    if (entity != entt::null) {
      if (const auto* drag = registry.try_get<comp::object_drag_info>(entity)) {
        const auto& object = registry.get<comp::object>(entity);
        _emit_event(dispatcher, *drag, object);
        registry.remove<comp::object_drag_info>(entity);
      }
    }
  }
}

}  // namespace tactile::sys
