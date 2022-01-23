#pragma once

#include <entt/entt.hpp>  // registry, entity
#include <tactile_def.hpp>

namespace tactile::sys {

/// \name Object layer system
/// \{

[[nodiscard]] auto HasObject(const entt::registry& registry,
                             entt::entity entity,
                             ObjectID id) -> bool;

[[nodiscard]] auto FindObject(const entt::registry& registry,
                              entt::entity entity,
                              ObjectID id) -> entt::entity;

/**
 * \brief Attempts to find an object in the specified layer that contains the supplied
 * coordinates.
 *
 * \details This function is designed to be used to find objects at the location that the
 * user has clicked in the map. As a result, this function takes care of the relationship
 * between viewport coordinates and logical coordinates. It also handles aspects such as
 * points having no width/height, so that points can be detected using this approach as
 * well.
 *
 * \param registry the current map registry.
 * \param entity the object layer entity.
 * \param x the viewport relative x-coordinate.
 * \param y the viewport relative y-coordinate.
 *
 * \return an object entity that contains the specified coordinates; a null entity is
 * returned if there is none.
 */
[[nodiscard]] auto FindObject(const entt::registry& registry,
                              entt::entity entity,
                              float x,
                              float y) -> entt::entity;

/// \} End of object layer system

}  // namespace tactile::sys
