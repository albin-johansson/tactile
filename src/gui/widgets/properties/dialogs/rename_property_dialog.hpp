#pragma once

#include <entt.hpp>  // registry, dispatcher
#include <string>    // string

namespace Tactile {

void UpdateRenamePropertyDialog(const entt::registry& registry,
                                entt::dispatcher& dispatcher);

void OpenRenamePropertyDialog(std::string name);

}  // namespace Tactile
