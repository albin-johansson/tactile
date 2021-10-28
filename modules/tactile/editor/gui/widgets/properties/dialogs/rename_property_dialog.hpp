#pragma once

#include <string>  // string

#include <entt.hpp>  // registry, dispatcher

namespace Tactile {

void UpdateRenamePropertyDialog(const entt::registry& registry,
                                entt::dispatcher& dispatcher);

void OpenRenamePropertyDialog(std::string name);

}  // namespace Tactile
