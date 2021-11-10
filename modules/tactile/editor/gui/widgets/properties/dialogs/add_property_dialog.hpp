#pragma once

#include <entt/entt.hpp>  // registry, dispatcher

namespace Tactile {

void UpdateAddPropertyDialog(const entt::registry& registry,
                             entt::dispatcher& dispatcher);

void OpenAddPropertyDialog();

}  // namespace Tactile
