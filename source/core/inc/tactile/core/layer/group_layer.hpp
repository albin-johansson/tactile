// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/vector.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Registry;

/// \addtogroup GroupLayer
/// \{

/**
 * A component that represents a layer variant that stores other layers.
 */
struct CGroupLayer final
{
  /** The stored layers. */
  Vector<EntityID> layers;
};

/**
 * Indicates whether an entity is a group layer.
 *
 * \details
 * Group layer entities feature the following components. \n
 * - \c CMeta \n
 * - \c CLayer \n
 * - \c CGroupLayer
 *
 * \param registry The associated registry.
 * \param entity   The entity to check.
 *
 * \return
 * True if the entity is a group layer; false otherwise.
 */
[[nodiscard]]
auto is_group_layer(const Registry& registry, EntityID entity) -> bool;

/**
 * Creates an empty group layer.
 *
 * \param registry The associated registry.
 *
 * \return
 * A group layer entity.
 */
[[nodiscard]]
auto make_group_layer(Registry& registry) -> EntityID;

/**
 * Destroys a group layer along with all of its nested layers.
 *
 * \pre The specified entity must be a group layer.
 *
 * \param registry           The associated registry.
 * \param group_layer_entity The group layer entity to destroy.
 */
void destroy_group_layer(Registry& registry, EntityID group_layer_entity);

/// \}

}  // namespace tactile
