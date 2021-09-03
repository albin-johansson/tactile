#pragma once

#include <entt.hpp>  // registry

namespace Tactile::Sys {

/// \name Map system
/// \{

void AddRow(entt::registry& registry);

void AddColumn(entt::registry& registry);

void RemoveRow(entt::registry& registry);

void RemoveColumn(entt::registry& registry);

/// \} End of map system

}  // namespace Tactile::Sys
