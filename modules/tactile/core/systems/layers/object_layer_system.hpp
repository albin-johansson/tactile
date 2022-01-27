#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/components/layer.hpp"

namespace tactile::sys {

/**
 * \ingroup layer-system
 * \defgroup object-layer-system Object Layer System
 */

/// \addtogroup object-layer-system
/// \{

/// \name Object functions
/// \{

[[nodiscard]] auto has_object(const entt::registry& registry,
                              const comp::object_layer& layer,
                              ObjectID id) -> bool;

[[nodiscard]] auto find_object(const entt::registry& registry,
                               const comp::object_layer& layer,
                               ObjectID id) -> entt::entity;

/**
 * \brief Finds an object in the specified layer that contains the supplied coordinates.
 *
 * \details This function is designed to be used to find objects at the location that the
 * user has clicked in the map. As a result, this function takes care of the relationship
 * between viewport coordinates and logical coordinates. It also handles aspects such as
 * points having no width/height, so that points can be detected using this approach as
 * well.
 *
 * \param registry the current map registry.
 * \param layer the source object layer.
 * \param x the viewport relative x-coordinate.
 * \param y the viewport relative y-coordinate.
 *
 * \return an object entity that contains the specified coordinates;
 *         a null entity is returned if there is none.
 */
[[nodiscard]] auto find_object(const entt::registry& registry,
                               const comp::object_layer& layer,
                               float x,
                               float y) -> entt::entity;

/// \} End of object functions

/// \} End of group object-layer-system

}  // namespace tactile::sys
