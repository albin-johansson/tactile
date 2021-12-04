#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

namespace Tactile {

void UpdatePropertiesDock(const entt::registry& registry, entt::dispatcher& dispatcher);

[[nodiscard]] auto IsPropertyDockFocused() noexcept -> bool;

}  // namespace Tactile