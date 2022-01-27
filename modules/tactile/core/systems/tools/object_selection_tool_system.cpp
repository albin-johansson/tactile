#include "object_selection_tool_system.hpp"

#include "core/components/object.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return is_object_layer_active(registry);
}

[[nodiscard]] auto GetTargetObject(const entt::registry& registry, const MouseInfo& mouse)
    -> entt::entity
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    return registry.ctx<comp::active_object>().entity;
  }
  else {
    return entt::null;
  }
}

}  // namespace

void ObjectSelectionToolOnPressed(entt::registry& registry,
                                  entt::dispatcher& dispatcher,
                                  const MouseInfo& mouse)
{
  if (IsUsable(registry)) {
    if (mouse.button == cen::mouse_button::left) {
      auto& active = registry.ctx<comp::active_object>();
      active.entity = entt::null;

      const auto layerEntity = get_active_layer(registry);
      const auto& layer = registry.get<ObjectLayer>(layerEntity);

      const auto objectEntity = find_object(registry, layer, mouse.x, mouse.y);
      if (objectEntity != entt::null) {
        const auto& object = registry.get<comp::object>(objectEntity);

        auto& drag = registry.emplace<comp::object_drag_info>(objectEntity);
        drag.origin_object_x = object.x;
        drag.origin_object_y = object.y;
        drag.last_mouse_x = mouse.x;
        drag.last_mouse_y = mouse.y;

        active.entity = objectEntity;
        dispatcher.enqueue<InspectContextEvent>(objectEntity);
      }
    }
    else if (mouse.button == cen::mouse_button::right) {
      auto& active = registry.ctx<comp::active_object>();
      active.entity = entt::null;

      const auto layerEntity = get_active_layer(registry);
      const auto& layer = registry.get<ObjectLayer>(layerEntity);

      const auto objectEntity = find_object(registry, layer, mouse.x, mouse.y);
      if (objectEntity != entt::null) {
        active.entity = objectEntity;
        dispatcher.enqueue<SpawnObjectContextMenuEvent>(objectEntity);
      }
    }
  }
}

void ObjectSelectionToolOnDragged(entt::registry& registry, const MouseInfo& mouse)
{
  const auto entity = GetTargetObject(registry, mouse);
  if (entity != entt::null) {
    auto& drag = registry.get<comp::object_drag_info>(entity);

    const auto [xRatio, yRatio] = GetViewportScalingRatio(registry);
    const auto dx = (mouse.x - drag.last_mouse_x) / xRatio;
    const auto dy = (mouse.y - drag.last_mouse_y) / yRatio;

    auto& object = registry.get<comp::object>(entity);
    object.x += dx;
    object.y += dy;

    drag.last_mouse_x = mouse.x;
    drag.last_mouse_y = mouse.y;
  }
}

void ObjectSelectionToolOnReleased(entt::registry& registry,
                                   entt::dispatcher& dispatcher,
                                   const MouseInfo& mouse)
{
  const auto entity = GetTargetObject(registry, mouse);
  if (entity != entt::null) {
    if (const auto* drag = registry.try_get<comp::object_drag_info>(entity)) {
      const auto& object = registry.get<comp::object>(entity);

      /* Only emit an event if the object has been moved along any axis */
      if (drag->origin_object_x != object.x || drag->origin_object_y != object.y) {
        dispatcher.enqueue<MoveObjectEvent>(object.id,
                                            drag->origin_object_x,
                                            drag->origin_object_y,
                                            object.x,
                                            object.y);
      }
    }

    registry.remove<comp::object_drag_info>(entity);
  }
}

}  // namespace tactile::sys
