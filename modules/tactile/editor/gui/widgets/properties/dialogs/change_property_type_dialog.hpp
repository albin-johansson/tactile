#pragma once

#include <string>  // string

#include <entt.hpp>  // dispatcher

#include <tactile-base/property_type.hpp>

namespace Tactile {

void UpdateChangePropertyTypeDialog(entt::dispatcher& dispatcher);

void OpenChangePropertyTypeDialog(std::string name, PropertyType type);

}  // namespace Tactile
