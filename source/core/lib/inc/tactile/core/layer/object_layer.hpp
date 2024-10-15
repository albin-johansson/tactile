// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class Registry;

/**
 * Indicates whether an entity is an object layer.
 *
 * \details
 * Object layer entities feature the following components. \n
 * - \c CMeta \n
 * - \c CLayer \n
 * - \c CObjectLayer
 *
 * \param registry The associated registry.
 * \param entity   The entity to check.
 *
 * \return
 * True if the entity is an object layer; false otherwise.
 */
[[nodiscard]]
auto is_object_layer(const Registry& registry, EntityID entity) -> bool;

/**
 * Creates an empty object layer.
 *
 * \param registry The associated registry.
 *
 * \return
 * An object layer entity.
 */
[[nodiscard]]
auto make_object_layer(Registry& registry) -> EntityID;

/**
 * Destroys an object layer and all of its associated objects.
 *
 * \param registry            The associated registry.
 * \param object_layer_entity The object layer to destroy.
 *
 * \pre The specified entity must be a valid object layer.
 */
void destroy_object_layer(Registry& registry, EntityID object_layer_entity);

}  // namespace tactile::core
