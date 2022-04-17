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

#include <optional>  // optional
#include <string>    // string
#include <utility>   // pair

#include <entt/entt.hpp>

#include "core/components/layers.hpp"
#include "core/map_info.hpp"
#include "core/systems/snapshot.hpp"
#include "tactile.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup layer-system Layer System
 */

/// \addtogroup layer-system
/// \{

/**
 * \brief Creates a basic layer entity.
 *
 * \pre `parent` must either be associated with a group layer or be null.
 *
 * \details The created entity will feature the following components:
 * - `AttributeContext`
 * - `Layer`
 * - `LayerTreeNode`
 * - `Parent`
 *
 * \param registry the associated registry.
 * \param id the unique identifier associated with the layer.
 * \param type the specific layer type.
 * \param name the name of the layer.
 * \param parent the parent layer entity, can safely be a null entity.
 *
 * \return the created layer entity.
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
 * - `AttributeContext`
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
 * - `AttributeContext`
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
 * - `AttributeContext`
 *
 * \param registry a map registry.
 *
 * \return the created group layer entity.
 */
auto make_group_layer(entt::registry& registry) -> entt::entity;

/**
 * \brief Removes the layer associated with the specified ID.
 *
 * \pre `entity` must be associated with an existing layer.
 *
 * \details The active layer is is reset if the specified layer is active at the time
 * of invocation. This function will also repair the layer indices.
 *
 * \param registry a map registry.
 * \param entity the layer entity that will be removed.
 *
 * \return a snapshot of the removed layer.
 */
auto remove_layer(entt::registry& registry, entt::entity entity) -> LayerSnapshot;

auto restore_layer(entt::registry& registry, LayerSnapshot snapshot) -> entt::entity;

auto duplicate_layer(entt::registry& registry, entt::entity source) -> entt::entity;

auto duplicate_layer(entt::registry& registry,
                     entt::entity source,
                     entt::entity parent,
                     bool recursive) -> entt::entity;

/**
 * \brief Selects the layer associated with the specified ID.
 *
 * \param registry the document registry.
 * \param id the ID of the layer that will be selected.
 *
 * \throws TactileError if there is no layer with the specified ID.
 */
void select_layer(entt::registry& registry, LayerID id);

[[nodiscard]] auto get_active_layer(const entt::registry& registry) -> entt::entity;

[[nodiscard]] auto get_active_layer_id(const entt::registry& registry)
    -> std::optional<LayerID>;

/**
 * \brief Attempts to find the layer associated with the specified ID.
 *
 * \param registry a map registry.
 * \param id the ID associated with the desired layer.
 *
 * \return the associated layer entity; a null entity is returned if there is none.
 */
[[nodiscard]] auto find_layer(const entt::registry& registry, LayerID id) -> entt::entity;

[[nodiscard]] auto get_layer_entity(const entt::registry& registry, LayerID id)
    -> entt::entity;

[[nodiscard]] auto get_layer(entt::registry& registry, LayerID id)
    -> std::pair<entt::entity, comp::Layer&>;

[[nodiscard]] auto get_layer(const entt::registry& registry, LayerID id)
    -> std::pair<entt::entity, const comp::Layer&>;

[[nodiscard]] auto is_tile_layer_active(const entt::registry& registry) -> bool;

[[nodiscard]] auto is_object_layer_active(const entt::registry& registry) -> bool;

/// \} End of group layer-system

}  // namespace tactile::sys
