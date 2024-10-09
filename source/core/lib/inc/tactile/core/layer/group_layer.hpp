// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>   // size_t
#include <optional>  // optional
#include <vector>    // vector

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

class Registry;

/**
 * A component that represents a layer variant that stores other layers.
 */
struct CGroupLayer final
{
  /** The stored layers. */
  std::vector<EntityID> layers;
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
 * \pre \c group_layer_id must reference a group layer.
 *
 * \param registry       The associated registry.
 * \param group_layer_id The target group layer identifier.
 */
void destroy_group_layer(Registry& registry, EntityID group_layer_id);

/**
 * Returns the number of layers in a group layer hierarchy, excluding the root.
 *
 * \pre \c root_layer_id must reference a group layer.
 *
 * \param registry      The associated registry.
 * \param root_layer_id The root group layer identifier.
 *
 * \return
 * The number of layers stored in the hierarchy.
 */
[[nodiscard]]
auto count_layers(const Registry& registry, EntityID root_layer_id) -> std::size_t;

/**
 * Attempts to find a layer parent within a group layer hierarchy.
 *
 * \pre \c root_layer_id must reference a group layer.
 * \pre \c target_layer_id must reference a layer.
 *
 * \param registry        The associated registry.
 * \param root_layer_id   The root group layer identifier.
 * \param target_layer_id The target layer identifier.
 *
 * \return
 * A parent layer identifier if found; an invalid entity otherwise.
 */
[[nodiscard]]
auto find_parent_layer(const Registry& registry,
                       EntityID root_layer_id,
                       EntityID target_layer_id) -> EntityID;

/**
 * Returns the local index of a layer within a group layer hierarchy.
 *
 * \pre \c root_layer_id must reference a group layer.
 * \pre \c target_layer_id must reference a layer.
 *
 * \param registry        The associated registry.
 * \param root_layer_id   The root group layer identifier.
 * \param target_layer_id The target layer identifier.
 *
 * \return
 * The local index of the layer if found; an empty optional otherwise.
 */
[[nodiscard]]
auto get_local_layer_index(const Registry& registry,
                           EntityID root_layer_id,
                           EntityID target_layer_id) -> std::optional<std::size_t>;

/**
 * Returns the global index of a layer within a group layer hierarchy.
 *
 * \pre \c root_layer_id must reference a group layer.
 * \pre \c target_layer_id must reference a layer.
 *
 * \param registry        The associated registry.
 * \param root_layer_id   The root group layer identifier.
 * \param target_layer_id The target layer identifier.
 *
 * \return
 * The global index of the layer if found; an empty optional otherwise.
 */
[[nodiscard]]
auto get_global_layer_index(const Registry& registry,
                            EntityID root_layer_id,
                            EntityID target_layer_id) -> std::optional<std::size_t>;

/**
 * Moves a layer up within a group layer hierarchy.
 *
 * \pre \c root_layer_id must reference a group layer.
 * \pre \c target_layer_id must reference a layer.
 * \pre The target layer must be movable upward.
 *
 * \param registry        The associated registry.
 * \param root_layer_id   The root group layer identifier.
 * \param target_layer_id The target layer identifier.
 *
 * \see \c can_move_layer_up
 */
void move_layer_up(Registry& registry, EntityID root_layer_id, EntityID target_layer_id);

/**
 * Moves a layer down within a group layer hierarchy.
 *
 * \pre \c root_layer_id must reference a group layer.
 * \pre \c target_layer_id must reference a layer.
 * \pre The target layer must be movable downward.
 *
 * \param registry        The associated registry.
 * \param root_layer_id   The root group layer identifier.
 * \param target_layer_id The target layer identifier.
 *
 * \see \c can_move_layer_down
 */
void move_layer_down(Registry& registry, EntityID root_layer_id, EntityID target_layer_id);

/**
 * Indicates whether a layer can be moved up in a group layer hierarchy.
 *
 * \pre \c root_layer_id must reference a group layer.
 * \pre \c target_layer_id must reference a layer.
 *
 * \param registry        The associated registry.
 * \param root_layer_id   The root group layer identifier.
 * \param target_layer_id The target layer identifier.
 *
 * \return
 * True if the layer can be moved; false otherwise.
 */
[[nodiscard]]
auto can_move_layer_up(const Registry& registry,
                       EntityID root_layer_id,
                       EntityID target_layer_id) -> bool;

/**
 * Indicates whether a layer can be moved down in a group layer hierarchy.
 *
 * \pre \c root_layer_id must reference a group layer.
 * \pre \c target_layer_id must reference a layer.
 *
 * \param registry        The associated registry.
 * \param root_layer_id   The root group layer identifier.
 * \param target_layer_id The target layer identifier.
 *
 * \return
 * True if the layer can be moved; false otherwise.
 */
[[nodiscard]]
auto can_move_layer_down(const Registry& registry,
                         EntityID root_layer_id,
                         EntityID target_layer_id) -> bool;

}  // namespace tactile::core
