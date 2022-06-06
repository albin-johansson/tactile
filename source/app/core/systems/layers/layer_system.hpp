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

#include <string>   // string
#include <utility>  // pair

#include <entt/fwd.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/components/layers.hpp"
#include "core/components/map_info.hpp"
#include "core/systems/snapshot.hpp"

namespace tactile::sys {

/**
 * Creates a basic layer entity.
 *
 * This function is used internally by the other layer factory functions, and does not
 * produce ready-to-use layer entities. The purpose of exposing this function is to make
 * restoring documents post parsing easier.
 *
 * \pre `parent` is either a group layer or null.
 *
 * The created entity will feature the following components:
 * - `comp::Context`
 * - `comp::Layer`
 * - `comp::LayerTreeNode`
 * - `comp::Parent`
 *
 * \param registry the associated registry.
 * \param type the specific layer type.
 * \param name the name of the layer.
 * \param parent the parent layer entity, can safely be null.
 *
 * \return the created layer entity.
 */
auto new_layer_skeleton(entt::registry& registry,
                        LayerType type,
                        std::string name,
                        entt::entity parent = entt::null) -> entt::entity;

/**
 * Creates a tile layer entity.
 *
 * The created layer will be added as a child layer if the active layer at the time of
 * invocation is a group layer.
 *
 * The created entity will feature the following components:
 * - `comp::Layer`
 * - `comp::LayerTreeNode`
 * - `comp::TileLayer`
 * - `comp::Parent`
 * - `comp::Context`
 *
 * \param registry the document registry.
 *
 * \return the created entity.
 */
auto new_tile_layer(entt::registry& registry) -> entt::entity;

/**
 * Creates an object layer entity.
 *
 * The created layer will be added as a child layer if the active layer at the time of
 * invocation is a group layer.
 *
 * The created entity will feature the following components:
 * - `comp::Layer`
 * - `comp::LayerTreeNode`
 * - `comp::ObjectLayer`
 * - `comp::Parent`
 * - `comp::Context`
 *
 * \param registry the document registry.
 *
 * \return the created entity.
 */
auto new_object_layer(entt::registry& registry) -> entt::entity;

/**
 * Creates a group layer entity.
 *
 * The created layer will be added as a child layer if the active layer at the time of
 * invocation is a group layer.
 *
 * The created entity will feature the following components:
 * - `comp::Layer`
 * - `comp::LayerTreeNode`
 * - `comp::GroupLayer`
 * - `comp::Parent`
 * - `comp::Context`
 *
 * \param registry the document registry.
 *
 * \return the created entity.
 */
auto new_group_layer(entt::registry& registry) -> entt::entity;

/**
 * Removes the layer associated with the specified ID.
 *
 * \pre `entity` must be associated with an existing layer.
 *
 * The active layer is is reset if the specified layer is active at the time of
 * invocation. This function will also repair the layer indices.
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
 * Selects the layer associated with the specified ID.
 *
 * \param registry the document registry.
 * \param id the ID of the layer that will be selected.
 *
 * \throws TactileError if there is no layer with the specified ID.
 */
void select_layer(entt::registry& registry, const UUID& id);

[[nodiscard]] auto get_active_layer(const entt::registry& registry) -> entt::entity;

[[nodiscard]] auto get_active_layer_id(const entt::registry& registry) -> Maybe<UUID>;

[[nodiscard]] auto is_tile_layer_active(const entt::registry& registry) -> bool;

[[nodiscard]] auto is_object_layer_active(const entt::registry& registry) -> bool;

}  // namespace tactile::sys
