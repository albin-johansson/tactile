#include "object_selection_tool_system.hpp"

#include "core/components/object.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "editor/events/property_events.hpp"

namespace Tactile::Sys {
namespace {

[[nodiscard]] auto IsUsable(const entt::registry& registry) -> bool
{
  return IsObjectLayerActive(registry);
}

}  // namespace

void ObjectSelectionToolOnPressed(entt::registry& registry,
                                  entt::dispatcher& dispatcher,
                                  const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    auto& active = registry.ctx<ActiveObject>();
    active.entity = entt::null;

    const auto layerEntity = Sys::GetActiveLayer(registry);
    const auto objectEntity = FindObject(registry, layerEntity, mouse.x, mouse.y);
    if (objectEntity != entt::null) {
      const auto& object = registry.get<Object>(objectEntity);

      auto& drag = registry.emplace<ObjectDragInfo>(objectEntity);
      drag.origin_object_x = object.x;
      drag.origin_object_y = object.y;
      drag.last_mouse_x = mouse.x;
      drag.last_mouse_y = mouse.y;

      active.entity = objectEntity;
      dispatcher.enqueue<SetPropertyContextEvent>(objectEntity);
    }
  }
}

void ObjectSelectionToolOnDragged(entt::registry& registry, const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    auto& active = registry.ctx<ActiveObject>();
    if (active.entity != entt::null && registry.all_of<ObjectDragInfo>(active.entity)) {
      auto& drag = registry.get<ObjectDragInfo>(active.entity);

      const auto [xRatio, yRatio] = Sys::GetViewportScalingRatio(registry);
      const auto dx = (mouse.x - drag.last_mouse_x) / xRatio;
      const auto dy = (mouse.y - drag.last_mouse_y) / yRatio;

      auto& object = registry.get<Object>(active.entity);
      object.x += dx;
      object.y += dy;

      drag.last_mouse_x = mouse.x;
      drag.last_mouse_y = mouse.y;
    }
  }
}

void ObjectSelectionToolOnReleased(entt::registry& registry,
                                   entt::dispatcher& dispatcher,
                                   const MouseInfo& mouse)
{
  if (IsUsable(registry) && mouse.button == cen::mouse_button::left) {
    auto& active = registry.ctx<ActiveObject>();
    if (active.entity != entt::null) {
      if (registry.remove<ObjectDragInfo>(active.entity) > 0) {
        // TODO register a "MoveObjectCmd" command
      }
    }
  }
}

}  // namespace Tactile::Sys
