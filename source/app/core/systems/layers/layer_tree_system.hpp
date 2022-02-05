#pragma once

#include <entt/entt.hpp>

#include "tactile.hpp"

namespace tactile::sys {

/**
 * \ingroup layer-system
 * \defgroup layer-tree-system Layer Tree System
 */

/// \addtogroup layer-tree-system
/// \{

void sort_layers(entt::registry& registry);

void increment_layer_indices_of_siblings_below(entt::registry& registry,
                                               entt::entity entity);

void decrement_layer_indices_of_siblings_below(entt::registry& registry,
                                               entt::entity entity);

void destroy_layer_node(entt::registry& registry, entt::entity entity);

void move_layer_up(entt::registry& registry, entt::entity entity);

void move_layer_down(entt::registry& registry, entt::entity entity);

[[nodiscard]] auto can_move_layer_up(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto can_move_layer_down(const entt::registry& registry,
                                       entt::entity entity) -> bool;

[[nodiscard]] auto is_child_layer_node(const entt::registry& registry,
                                       entt::entity parent,
                                       entt::entity entity) -> bool;

[[nodiscard]] auto layer_sibling_above(const entt::registry& registry,
                                       entt::entity entity) -> entt::entity;

[[nodiscard]] auto layer_sibling_below(const entt::registry& registry,
                                       entt::entity entity) -> entt::entity;

[[nodiscard]] auto layer_sibling_count(const entt::registry& registry,
                                       entt::entity entity) -> usize;

[[nodiscard]] auto layer_children_count(const entt::registry& registry,
                                        entt::entity entity) -> usize;

[[nodiscard]] auto layer_local_index(const entt::registry& registry, entt::entity entity)
    -> usize;

[[nodiscard]] auto layer_global_index(const entt::registry& registry, entt::entity entity)
    -> usize;

/// \} End of group layer-tree-system

}  // namespace tactile::sys