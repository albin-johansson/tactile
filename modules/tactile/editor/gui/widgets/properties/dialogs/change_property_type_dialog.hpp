#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

void UpdateChangePropertyTypeDialog(entt::dispatcher& dispatcher);

void OpenChangePropertyTypeDialog(std::string name, PropertyType type);

}  // namespace Tactile
