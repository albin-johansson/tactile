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

#include "core/ecs/entity.hpp"
#include "core/ecs/registry.hpp"
#include "core/functional/result.hpp"
#include "core/tiles/tile_pos.hpp"
#include "model/layers/layer_components.hpp"
#include "tactile/core/common/prelude.hpp"
#include "tactile/core/containers/vector.hpp"
#include "tactile/core/functional/maybe.hpp"

namespace tactile::sys {

/**
 * Runs a flood-fill algorithm in the layer, used by the bucket tool.
 *
 * \details This will replace all adjacent tiles of the type at the origin position with
 *          the specified tile replacement.
 *
 * \param tile_layer    the target tile layer.
 * \param origin        the initial position of the flood.
 * \param replacement   the tile ID that will be used when replacing tiles.
 * \param[out] affected optional destination for all modified positions.
 */
void flood_tiles(TileLayer& tile_layer,
                 const TilePos& origin,
                 TileID replacement,
                 Vector<TilePos>* affected = nullptr);

/**
 * Sets the tile ID in a tile layer.
 *
 * \note This function does nothing if the provided position is out-of-bounds.
 *
 * \param tile_layer the target tile layer.
 * \param pos        the position of the tile to modify.
 * \param tile_id    the new tile identifier.
 *
 * \return success if the tile was set; failure otherwise.
 */
auto set_tile(TileLayer& tile_layer, TilePos pos, TileID tile_id) -> Result;

/// Returns the tile identifier at the specified position, as long as it's valid.
[[nodiscard]] auto tile_at(const TileLayer& tile_layer, TilePos pos) -> Maybe<TileID>;

/// Indicates whether a position is a valid tile position in the tile layer.
[[nodiscard]] auto is_valid_tile(const TileLayer& tile_layer, TilePos pos) -> bool;

}  // namespace tactile::sys
