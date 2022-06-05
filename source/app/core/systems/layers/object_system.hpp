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

#include <entt/fwd.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/math.hpp"
#include "core/common/uuid.hpp"
#include "core/fwd.hpp"
#include "core/systems/snapshot.hpp"

namespace tactile::sys {

struct RemoveObjectResult final
{
  UUID layer{};
  comp::Object object;
  AttributeContextSnapshot context;
};

/**
 * Adds a new rectangle to an object layer.
 *
 * The created entity will feature the following components:
 * - `comp::Object`
 * - `comp::Context`
 *
 * \param registry the document registry.
 * \param layerId the object layer to add the object to.
 * \param pos the object position.
 * \param size the object size.
 *
 * \return the identifier assigned to the object.
 */
auto new_rectangle_object(entt::registry& registry,
                          const UUID& layerId,
                          const Vector2f& pos,
                          const Vector2f& size) -> ObjectID;

/**
 * Adds a new ellipse to an object layer.
 *
 * The created entity will feature the following components:
 * - `comp::Object`
 * - `comp::Context`
 *
 * \param registry the document registry.
 * \param layerId the object layer to add the object to.
 * \param pos the object position.
 * \param size the object size.
 *
 * \return the identifier assigned to the object.
 */
auto new_ellipse_object(entt::registry& registry,
                        const UUID& layerId,
                        const Vector2f& pos,
                        const Vector2f& size) -> ObjectID;

/**
 * Adds a new point to an object layer.
 *
 * \pre The specified layer must be an object layer.
 *
 * The created entity will feature the following components:
 * - `comp::Object`
 * - `comp::Context`
 *
 * \param registry the map registry.
 * \param layerId the object layer to add the object to.
 * \param pos the object position.
 *
 * \return the identifier assigned to the object.
 */
auto new_point_object(entt::registry& registry, const UUID& layerId, const Vector2f& pos)
    -> ObjectID;

/**
 * Removes an object.
 *
 * This also removes the object from its associated object layer.
 *
 * \param registry the map registry.
 * \param id the identifier associated with the object to remove.
 *
 * \return a snapshot of the removed object.
 *
 * \throws TactileError if the identifier is invalid.
 */
auto remove_object(entt::registry& registry, ObjectID id) -> RemoveObjectResult;

/**
 * Restores a previously removed object.
 *
 * \param registry the map registry.
 * \param snapshot the snapshot information.
 */
void restore_object(entt::registry& registry, RemoveObjectResult snapshot);

/**
 * Returns the object entity associated with a specific ID.
 *
 * \param registry the document registry.
 * \param id the ID associated with the desired object.
 *
 * \return the found entity;
 *         a null entity if no object is found.
 */
[[nodiscard]] auto find_object(const entt::registry& registry, ObjectID id)
    -> entt::entity;

/**
 * Returns the object entity associated with a specific ID.
 *
 * \param registry the document registry.
 * \param id the ID of the desired object.
 *
 * \return the found entity.
 *
 * \throws TactileError if the identifier is invalid.
 */
[[nodiscard]] auto get_object(const entt::registry& registry, ObjectID id)
    -> entt::entity;

}  // namespace tactile::sys