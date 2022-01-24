#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/tile_position.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup map-system Map System
 */

/// \addtogroup map-system
/// \{

/**
 * \brief Appends a row of empty tiles to a map.
 *
 * \param registry the map registry.
 */
void add_row_to_map(entt::registry& registry);

/**
 * \brief Appends a column of empty tiles to a map.
 *
 * \param registry the map registry.
 */
void add_column_to_map(entt::registry& registry);

/**
 * \brief Removes the last row of tiles from a map.
 *
 * \details This function has no effect if there is only one row in the map.
 *
 * \param registry the map registry.
 */
void remove_row_from_map(entt::registry& registry);

/**
 * \brief Removes the last column of tiles from a map.
 *
 * \details This function has no effect if there is only one column in the map.
 *
 * \param registry the map registry.
 */
void remove_column_from_map(entt::registry& registry);

/**
 * \brief Sets the size of a map.
 *
 * \param registry the map registry.
 * \param nRows the new number of rows.
 * \param nCols the new number of columns.
 */
void resize_map(entt::registry& registry, usize nRows, usize nCols);

/**
 * \brief Indicates whether a position is within the bounds of a map.
 *
 * \param registry the map registry.
 * \param position the position that will be checked.
 *
 * \return `true` if the position is within the map; `false` otherwise.
 */
[[nodiscard]] auto is_position_in_map(const entt::registry& registry,
                                      const tile_position& position) -> bool;

/// \} End of group map-system

}  // namespace tactile::sys
