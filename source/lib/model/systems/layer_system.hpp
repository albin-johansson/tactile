/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/numeric.hpp"
#include "common/type/ecs.hpp"
#include "common/type/func.hpp"
#include "core/layer.hpp"
#include "core/tile_extent.hpp"
#include "model/model.hpp"

namespace tactile::sys {

using EntityCallback = Func<void(Entity)>;
using TileLayerCallback = Func<void(Entity, TileLayer&)>;
using ConstTileLayerCallback = Func<void(Entity, const TileLayer&)>;

[[nodiscard]] auto is_layer_entity(const Model& model, Entity entity) -> bool;
[[nodiscard]] auto is_tile_layer_entity(const Model& model, Entity entity) -> bool;
[[nodiscard]] auto is_object_layer_entity(const Model& model, Entity entity) -> bool;
[[nodiscard]] auto is_group_layer_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto create_tile_layer(Model& model, int32 id, TileExtent extent) -> Entity;

[[nodiscard]] auto create_object_layer(Model& model, int32 id) -> Entity;

[[nodiscard]] auto create_group_layer(Model& model, int32 id) -> Entity;

auto duplicate_layer(Model& model,
                     Entity root_layer_entity,
                     Entity source_layer_entity,
                     int32 new_id) -> Entity;

/// Recursively invokes a callback for each child layer, excluding the root group layer.
///
/// \param model the source model.
/// \param root_layer_entity a group layer entity representing the root of the search.
/// \param callback a function object invoked for each layer entity.
void recurse_layers(const Model& model,
                    Entity root_layer_entity,
                    const EntityCallback& callback);

void recurse_tile_layers(Model& model,
                         Entity root_layer_entity,
                         const TileLayerCallback& callback);

void recurse_tile_layers(const Model& model,
                         Entity root_layer_entity,
                         const ConstTileLayerCallback& callback);

[[nodiscard]] auto get_parent_layer(const Model& model,
                                    Entity root_layer_entity,
                                    Entity target_layer_entity) -> Entity;

}  // namespace tactile::sys
