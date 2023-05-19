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
#include "common/type/hash_map.hpp"
#include "common/type/math.hpp"
#include "common/type/result.hpp"
#include "core/layer.hpp"
#include "core/tile_extent.hpp"
#include "core/tile_pos.hpp"
#include "model/model.hpp"

namespace tactile::sys {

/// Maps layer entities to previously invalid tile identifiers.
using FixTilesInMapResult = HashMap<LayerEntity, HashMap<TilePos, TileID>>;

auto create_map(Model& model, const TileExtent& extent, const Int2& tile_size) -> Entity;

auto fix_tiles_in_map(Model& model, Entity map_entity) -> FixTilesInMapResult;

void restore_tiles_in_map(Model& model, const FixTilesInMapResult& invalid_tiles);

void add_row_to_map(Model& model, Entity map_entity);

void add_column_to_map(Model& model, Entity map_entity);

auto remove_row_from_map(Model& model, Entity map_entity) -> Result;

auto remove_column_from_map(Model& model, Entity map_entity) -> Result;

void resize_map(Model& model, Entity map_entity, TileExtent new_extent);

auto add_new_layer_to_map(Model& model, Entity map_entity, LayerType type) -> Entity;

void attach_layer_to_map(Model& model,
                         Entity map_entity,
                         Entity layer_entity,
                         Entity root_layer_entity = kNullEntity);

void remove_layer_from_map(Model& model, Entity map_entity, Entity layer_entity);

auto attach_tileset_to_map(Model& model, Entity map_entity, Entity tileset_entity)
    -> AttachedTilesetEntity;

[[nodiscard]] auto can_tile_row_be_removed(const Model& model) -> bool;
[[nodiscard]] auto can_tile_column_be_removed(const Model& model) -> bool;

}  // namespace tactile::sys
