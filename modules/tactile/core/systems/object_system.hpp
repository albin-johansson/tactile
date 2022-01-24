#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup object-system Object System
 */

/// \name Object system
/// \{

/**
 * \brief Returns the object entity associated with a specific ID.
 *
 * \ingroup object-system
 *
 * \param registry the document registry.
 * \param id the ID associated with the desired object.
 *
 * \return the found entity; a null entity is returned if no object is found.
 */
[[nodiscard]] auto find_object(const entt::registry& registry, ObjectID id)
    -> entt::entity;

/// \} End of object system

}  // namespace tactile::sys