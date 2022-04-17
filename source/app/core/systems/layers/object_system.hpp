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

#include "core/components/objects.hpp"
#include "core/systems/snapshot.hpp"
#include "tactile.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup object-system Object System
 */

/// \addtogroup object-system
/// \{

/**
 * \brief Adds a new rectangle object to the active object layer.
 *
 * \pre The layer must be an object layer.
 *
 * \details The created entity will feature the following components:
 * - `Object`
 * - `AttributeContext`
 *
 * \param registry the document registry.
 * \param layerId the object layer to add the object to.
 * \param x the rectangle x-coordinate.
 * \param y the rectangle y-coordinate.
 * \param width the rectangle width.
 * \param height the rectangle height.
 *
 * \return the identifier assigned to the object.
 */
auto new_rectangle_object(entt::registry& registry,
                          LayerID layerId,
                          float x,
                          float y,
                          float width,
                          float height) -> ObjectID;

/**
 * \brief Adds a new ellipse object to the active object layer.
 *
 * \pre The layer must be an object layer.
 *
 * \details The created entity will feature the following components:
 * - `Object`
 * - `AttributeContext`
 *
 * \param registry the document registry.
 * \param layerId the object layer to add the object to.
 * \param x the ellipse x-coordinate.
 * \param y the ellipse y-coordinate.
 * \param width the ellipse width.
 * \param height the ellipse height.
 *
 * \return the identifier assigned to the object.
 */
auto new_ellipse_object(entt::registry& registry,
                        LayerID layerId,
                        float x,
                        float y,
                        float width,
                        float height) -> ObjectID;

/**
 * \brief Adds a new point object to the active object layer.
 *
 * \pre The layer must be an object layer.
 *
 * \details The created entity will feature the following components:
 * - `Object`
 * - `AttributeContext`
 *
 * \param registry the map registry.
 * \param layerId the object layer to add the object to.
 * \param x the point x-coordinate.
 * \param y the point y-coordinate.
 *
 * \return the identifier assigned to the object.
 */
auto new_point_object(entt::registry& registry, LayerID layerId, float x, float y)
    -> ObjectID;

struct RemoveObjectResult final
{
  LayerID layer{};
  comp::Object object;
  sys::AttributeContextSnapshot context;
};

/**
 * \brief Removes an object.
 *
 * \details This function will also remove the object from the associated object layer.
 *
 * \param registry the map registry.
 * \param id the identifier associated with the object to remove.
 *
 * \return a snapshot of the removed object.
 *
 * \throws TactileError if the object ID is invalid.
 */
auto remove_object(entt::registry& registry, ObjectID id) -> RemoveObjectResult;

/**
 * \brief Restores a previously removed object.
 *
 * \param registry the map registry.
 * \param snapshot the snapshot information.
 */
void restore_object(entt::registry& registry, RemoveObjectResult snapshot);

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