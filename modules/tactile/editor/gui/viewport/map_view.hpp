#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

namespace tactile {

void UpdateMapView(const entt::registry& registry, entt::dispatcher& dispatcher);

void UpdateMapViewObjectContextMenu(const entt::registry& registry,
                                    entt::dispatcher& dispatcher);

void OpenObjectContextMenu();

}  // namespace tactile
