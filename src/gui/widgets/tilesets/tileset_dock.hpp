#pragma once

#include <entt.hpp>  // registry, dispatcher

namespace Tactile {

void UpdateTilesetDock(const entt::registry& registry, entt::dispatcher& dispatcher);

[[nodiscard]] auto IsTilesetDockFocused() noexcept -> bool;

}  // namespace Tactile
