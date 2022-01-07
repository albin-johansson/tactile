#pragma once

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/map_position.hpp"

namespace Tactile::Sys {

/// \name Map system
/// \{

/**
 * \brief Appends a row of empty tiles to a map.
 *
 * \param registry the map registry that will be modified.
 */
void AddRow(entt::registry& registry);

/**
 * \brief Appends a column of empty tiles to a map.
 *
 * \param registry the map registry that will be modified.
 */
void AddColumn(entt::registry& registry);

/**
 * \brief Removes the last row of tiles from a map.
 *
 * \details This function has no effect if there is only one row in the map.
 *
 * \param registry the map registry that will be modified.
 */
void RemoveRow(entt::registry& registry);

/**
 * \brief Removes the last column of tiles from a map.
 *
 * \details This function has no effect if there is only one column in the map.
 *
 * \param registry the map registry that will be modified.
 */
void RemoveColumn(entt::registry& registry);

/**
 * \brief Sets the size of a map.
 *
 * \param registry the map registry that will be modified.
 * \param nRows the new number of rows.
 * \param nCols the new number of columns.
 */
void ResizeMap(entt::registry& registry, usize nRows, usize nCols);

/**
 * \brief Indicates whether or not a position is within the bounds of a map.
 *
 * \param registry the map registry that will be queried.
 * \param position the position that will be checked.
 *
 * \return `true` if the position is within the map; `false` otherwise.
 */
[[nodiscard]] auto IsPositionInMap(const entt::registry& registry,
                                   const MapPosition& position) -> bool;

/// \} End of map system

}  // namespace Tactile::Sys
