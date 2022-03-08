/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <entt/entt.hpp>

#include "tactile.hpp"

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
[[nodiscard]] auto find_object(const entt::registry& registry, object_id id)
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
[[nodiscard]] auto get_object(const entt::registry& registry, object_id id)
    -> entt::entity;

/// \} End of group object-system

}  // namespace tactile::sys