#pragma once

#include <entt.hpp>  // registry, dispatcher
#include <string>    // string

namespace Tactile {

void UpdateChangePropertyTypeDialog(const entt::registry& registry,
                                    entt::dispatcher& dispatcher);

void OpenChangePropertyTypeDialog(std::string name);

}  // namespace Tactile
