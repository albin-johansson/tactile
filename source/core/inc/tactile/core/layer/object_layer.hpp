// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/vector.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Registry;

/// \addtogroup ObjectLayer
/// \{

/**
 * A component that represents a layer of objects.
 */
struct CObjectLayer final
{
  /** The associated objects. */
  Vector<EntityID> objects;
};

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
 * \pre The specified entity must be an object layer.
 *
 * \param registry            The associated registry.
 * \param object_layer_entity The object layer to destroy.
 */
void destroy_object_layer(Registry& registry, EntityID object_layer_entity);

/// \}

}  // namespace tactile
