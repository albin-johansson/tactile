#pragma once

#include <entt/entt.hpp>
#include <tactile_def.hpp>

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup object-system Object System
 */

/// \addtogroup object-system
/// \{

/**
 * \brief Returns the object entity associated with a specific ID.
 *
 * \param registry the document registry.
 * \param id the ID associated with the desired object.
 *
 * \return the found entity; a null entity is returned if no object is found.
 */
[[nodiscard]] auto find_object(const entt::registry& registry, ObjectID id)
    -> entt::entity;

/**
 * \brief Returns the object entity associated with a specific ID.
 *
 * \param registry the document registry.
 * \param id the ID associated with the desired object.
 *
 * \return the found entity.
 *
 * \throws TactileError if the object identifier is invalid.
 */
[[nodiscard]] auto get_object(const entt::registry& registry, ObjectID id)
    -> entt::entity;

/// \} End of group object-system

}  // namespace tactile::sys