#pragma once

#include <tactile-base/map_position.hpp>
#include <tactile-base/tactile_std.hpp>

#include <entt.hpp>  // registry

namespace Tactile::Sys {

/// \name Map system
/// \{

void AddRow(entt::registry& registry);

void AddColumn(entt::registry& registry);

void RemoveRow(entt::registry& registry);

void RemoveColumn(entt::registry& registry);

void ResizeMap(entt::registry& registry, usize nRows, usize nCols);

[[nodiscard]] auto IsPositionInMap(const entt::registry& registry,
                                   const MapPosition& position) -> bool;

/// \} End of map system

}  // namespace Tactile::Sys
