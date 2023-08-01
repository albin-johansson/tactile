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

#include "common/enum/layer_type.hpp"
#include "common/result.hpp"
#include "common/type/ecs.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/math.hpp"
#include "core/prelude.hpp"
#include "core/tiles/tile_extent.hpp"
#include "core/tiles/tile_pos.hpp"
#include "model/maps/map_components.hpp"
#include "model/registry.hpp"

namespace tactile::sys {

/// Maps layer entities to previously invalid tile identifiers.
using FixTilesInMapResult = HashMap<Entity, HashMap<TilePos, TileID>>;

/**
 * Fixes all invalid tile identifiers in a map.
 *
 * \details This function iterates all associated tile layers in the map and marks all
 *          tiles with invalid tile identifiers as empty. An invalid tile identifier is
 *          simply a tile identifier that isn't associated with any of the map's attached
 *          tilesets.
 *
 * \param model the associated registry.
 * \param map   the map with tile layers to scan.
 *
 * \return a snapshot of affected tile layer entities mapped to the the positions and
 *         previous tile identifiers of all modified tiles.
 */
auto fix_tiles_in_map(Registry& registry, Map& map) -> FixTilesInMapResult;

/**
 * Restores the state of tiles previously modified using `fix_tiles_in_map`.
 *
 * \param model         the associated registry.
 * \param invalid_tiles a snapshot of the previous tile state.
 *
 * \see fix_tiles_in_map
 */
void restore_tiles_in_map(Registry& registry, const FixTilesInMapResult& invalid_tiles);

/**
 * Appends an empty row of tiles to each tile layer in a map.
 *
 * \param model the associated registry.
 * \param map   the map that will be modified.
 */
void add_row_to_map(Registry& registry, Map& map);

/**
 * Appends an empty column of tiles to each tile layer in a map.
 *
 * \param model the associated registry.
 * \param map   the map that will be modified.
 */
void add_column_to_map(Registry& registry, Map& map);

/**
 * Removes a row of tiles from each tile layer in a map.
 *
 * \note This function does nothing if the map only consists of a single row.
 *
 * \param model the associated registry.
 * \param map   the map that will be modified.
 *
 * \return success if a row was removed; failure otherwise.
 */
auto remove_row_from_map(Registry& registry, Map& map) -> Result;

/**
 * Removes a column of tiles from each tile layer in a map.
 *
 * \note This function does nothing if the map only consists of a single column.
 *
 * \param model the associated registry.
 * \param map   the map that will be modified.
 *
 * \return success if a column was removed; failure otherwise.
 */
auto remove_column_from_map(Registry& registry, Map& map) -> Result;

/**
 * Changes the dimensions of a map.
 *
 * \note This function does nothing if the new extent isn't at least 1x1.
 *
 * \param model      the associated registry.
 * \param map        the map that will be modified.
 * \param new_extent the new size of the map.
 *
 * \return success if the provided extent was valid; failure otherwise.
 */
auto resize_map(Registry& registry, Map& map, TileExtent new_extent) -> Result;

/**
 * Creates a new layer and attaches it to a map.
 *
 * \details The parent of the new layer depends on the active layer in the map. The new
 *          layer is added to the active layer if the active layer is a group layer.
 *          The invisible root layer is used instead if this isn't the case.
 *
 * \param model      the associated registry.
 * \param map_entity a map entity.
 * \param type       the type of the new layer.
 *
 * \return a layer entity.
 */
auto add_new_layer_to_map(Registry& registry, Entity map_entity, LayerType type)
    -> Entity;

/**
 * Attaches a layer to a map.
 *
 * \details The invisible root layer is used as the parent of the layer if the provided
 *          root layer entity is null.
 *
 * \param model             the associated registry.
 * \param map               the target map.
 * \param layer_entity      the layer entity to attach.
 * \param root_layer_entity a group layer entity or null.
 */
void attach_layer_to_map(Registry& registry,
                         Map& map,
                         Entity layer_entity,
                         Entity root_layer_entity = kNullEntity);

auto duplicate_layer(Registry& registry, Entity map_entity, Entity src_layer_entity)
    -> Entity;

/**
 * Removes an existing layer from a map.
 *
 * \details The active layer property is reset if the removed layer is active when this
 *          function is called. This function has no effect if the layer isn't stored in
 *          the map.
 *
 * \param model        the associated registry.
 * \param map          the map containing the layer.
 * \param layer_entity a layer entity.
 */
void remove_layer_from_map(Registry& registry, Map& map, Entity layer_entity);

/**
 * Attaches a tileset to a map.
 *
 * \details This function creates an "attached tileset" and adjusts the next available
 *          tile identifier property associated with the map.
 *
 * \param model          the associated registry.
 * \param map_entity     a map entity.
 * \param tileset_entity a tileset entity.
 *
 * \return an attached tileset entity.
 */
auto attach_tileset_to_map(Registry& registry, Entity map_entity, Entity tileset_entity)
    -> Entity;

/**
 * Makes an attached tileset active in a map.
 *
 * \pre `attached_tileset_entity` must be a valid attached tileset entity.
 *
 * \param model                   the associated model instance.
 * \param map                     the target map.
 * \param attached_tileset_entity the attached tileset that will be selected.
 *
 * \return success if the tileset was selected; failure otherwise.
 */
auto select_tileset(Registry& registry, Map& map, Entity attached_tileset_entity)
    -> Result;

/// Indicates whether the 'remove row' action is currently available.
[[nodiscard]] auto can_tile_row_be_removed(const Registry& registry) -> bool;

/// Indicates whether the 'remove column' action is currently available.
[[nodiscard]] auto can_tile_column_be_removed(const Registry& registry) -> bool;

}  // namespace tactile::sys
