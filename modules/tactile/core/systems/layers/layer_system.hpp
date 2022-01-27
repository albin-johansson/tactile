#pragma once

#include <string>   // string
#include <utility>  // pair

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/components/layer.hpp"
#include "core/map.hpp"
#include "core/systems/snapshot.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup layer-system Layer System
 */

/// \addtogroup layer-system
/// \{

/// \name General layer functions
/// \{

/**
 * \brief Creates a basic layer entity.
 *
 * \pre `parent` is either associated with a group layer or null.
 *
 * \details The created entity will feature the following components:
 * - `Layer`
 * - `LayerTreeNode`
 * - `Parent`
 * - `attribute_context`
 *
 * \param registry the associated registry.
 * \param id the unique identifier associated with the layer.
 * \param type the specific layer type.
 * \param name the name of the layer.
 * \param parent the parent layer entity, can safely be a null entity.
 *
 * \return the created layer entity.
 *
 * \since 0.2.0
 */
auto make_basic_layer(entt::registry& registry,
                      LayerID id,
                      LayerType type,
                      std::string name,
                      entt::entity parent = entt::null) -> entt::entity;

/**
 * \brief Creates a tile layer entity.
 *
 * \details The created layer will be added as a child layer if the active layer at
 * the time of invocation is a group layer.
 *
 * \details The created entity will feature the following components:
 * - `Layer`
 * - `TileLayer`
 * - `Parent`
 * - `attribute_context`
 *
 * \param registry the associated registry.
 *
 * \return the created tile layer entity.
 *
 * \since 0.2.0
 */
auto make_tile_layer(entt::registry& registry) -> entt::entity;

/**
 * \brief Creates an object layer entity.
 *
 * \details The created layer will be added as a child layer if the active layer at
 * the time of invocation is a group layer.
 *
 * \details The created entity will feature the following components:
 * - `Layer`
 * - `ObjectLayer`
 * - `Parent`
 * - `attribute_context`
 *
 * \param registry the associated registry.
 *
 * \return the created object layer entity.
 *
 * \since 0.2.0
 */
auto make_object_layer(entt::registry& registry) -> entt::entity;

/**
 * \brief Creates a group layer entity.
 *
 * \details The created layer will be added as a child layer if the active layer at
 * the time of invocation is a group layer.
 *
 * \details The created entity will feature the following components:
 * - `Layer`
 * - `GroupLayer`
 * - `Parent`
 * - `attribute_context`
 *
 * \param registry the associated registry.
 *
 * \return the created group layer entity.
 *
 * \since 0.2.0
 */
auto make_group_layer(entt::registry& registry) -> entt::entity;

auto restore_layer(entt::registry& registry, LayerSnapshot snapshot) -> entt::entity;

/**
 * \brief Sorts all layers according to their current indices.
 *
 * \param registry the associated registry.
 *
 * \since 0.2.0
 */
void sort_layers(entt::registry& registry);

/**
 * \brief Removes the layer associated with the specified ID.
 *
 * \pre `entity` must be associated with an existing layer.
 *
 * \details The active layer is is reset if the specified layer is active at the time
 * of invocation. This function will also repair the layer indices.
 *
 * \param registry the associated registry.
 * \param entity the layer entity that will be removed.
 *
 * \since 0.2.0
 */
void remove_layer(entt::registry& registry, entt::entity entity);

[[nodiscard]] auto copy_layer(const entt::registry& registry, entt::entity source)
    -> LayerSnapshot;

auto duplicate_layer(entt::registry& registry, entt::entity source) -> entt::entity;

auto duplicate_layer(entt::registry& registry,
                     entt::entity source,
                     entt::entity parent,
                     bool recursive) -> entt::entity;

/**
 * \brief Moves the specified layer "up", i.e. earlier in the rendering order.
 *
 * \pre `entity` must be associated with an existing layer.
 * \pre It must be possible to move the specified layer upwards.
 *
 * \details This function will repair the layer indices after moving the layer.
 *
 * \param registry the associated registry.
 * \param entity the entity associated with the layer that will be moved up.
 *
 * \see `can_move_layer_up()`
 *
 * \since 0.2.0
 */
void move_layer_up(entt::registry& registry, entt::entity entity);

/**
 * \brief Moves the specified layer "down", i.e. later in the rendering order.
 *
 * \pre `entity` must be associated with an existing layer.
 * \pre It must be possible to move the specified layer down.
 *
 * \details This function will repair the layer indices after moving the layer.
 *
 * \param registry the associated registry.
 * \param entity the entity associated with the layer that will be moved downwards.
 *
 * \see `can_move_layer_down()`
 *
 * \since 0.2.0
 */
void move_layer_down(entt::registry& registry, entt::entity entity);

[[deprecated]] void set_layer_opacity(entt::registry& registry,
                                      entt::entity entity,
                                      float opacity);

[[deprecated]] void set_layer_visible(entt::registry& registry,
                                      entt::entity entity,
                                      bool visible);

/**
 * \brief Attempts to find the layer associated with the specified ID.
 *
 * \param registry the associated registry.
 * \param id the ID associated with the desired layer.
 *
 * \return the associated layer entity; a null entity is returned if there is none.
 *
 * \since 0.2.0
 */
[[nodiscard]] auto find_layer(const entt::registry& registry, LayerID id) -> entt::entity;

[[nodiscard]] auto get_layer(entt::registry& registry, LayerID id)
    -> std::pair<entt::entity, Layer&>;

[[nodiscard]] auto get_layer(const entt::registry& registry, LayerID id)
    -> std::pair<entt::entity, const Layer&>;

[[nodiscard]] auto get_active_layer(const entt::registry& registry) -> entt::entity;

[[nodiscard, deprecated]] auto get_layer_index(const entt::registry& registry,
                                               entt::entity entity) -> usize;

[[nodiscard]] auto can_move_layer_up(const entt::registry& registry, entt::entity entity)
    -> bool;

[[nodiscard]] auto can_move_layer_down(const entt::registry& registry,
                                       entt::entity entity) -> bool;

[[nodiscard]] auto is_tile_layer_active(const entt::registry& registry) -> bool;

[[nodiscard]] auto is_object_layer_active(const entt::registry& registry) -> bool;

[[nodiscard]] auto get_active_layer_id(const entt::registry& registry) -> Maybe<LayerID>;

/// \} End of general layer functions

/// \} End of group layer-system

}  // namespace tactile::sys
