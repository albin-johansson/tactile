#pragma once

#include <entt/entt.hpp>  // registry, entity
#include <tactile_def.hpp>

namespace Tactile::Sys {

/// \name Object system
/// \{

/**
 * \brief Returns the object entity associated with a specific ID.
 *
 * \param registry the registry that will be queried.
 * \param id the ID associated with the desired object entity.
 *
 * \return the found entity; the null entity is returned if no match is found.
 */
[[nodiscard]] auto FindObject(const entt::registry& registry, ObjectID id)
    -> entt::entity;

/// \} End of object system

}  // namespace Tactile::Sys