#pragma once

#include <entt.hpp>  // dispatcher
#include <string>    // string

#include "core/property_type.hpp"

namespace Tactile {

void UpdateChangePropertyTypeDialog(entt::dispatcher& dispatcher);

void OpenChangePropertyTypeDialog(std::string name, PropertyType type);

}  // namespace Tactile
