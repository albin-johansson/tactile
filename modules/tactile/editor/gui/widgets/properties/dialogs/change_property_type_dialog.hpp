#pragma once

#include <string>  // string

#include <tactile-base/property_type.hpp>

#include <entt.hpp>  // dispatcher

namespace Tactile {

void UpdateChangePropertyTypeDialog(entt::dispatcher& dispatcher);

void OpenChangePropertyTypeDialog(std::string name, PropertyType type);

}  // namespace Tactile
