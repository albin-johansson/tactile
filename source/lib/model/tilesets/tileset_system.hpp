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

#include "common/primitives.hpp"
#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "common/type/path.hpp"
#include "model/maps/map_components.hpp"
#include "model/registry.hpp"

namespace tactile::sys {

[[nodiscard]] auto get_tile_appearance(const Registry& registry,
                                       Entity tileset_entity,
                                       TileIndex tile_index) -> TileIndex;

[[nodiscard]] auto is_valid_tile_identifier(const Registry& registry,
                                            const Map& map,
                                            TileID tile_id) -> bool;

[[nodiscard]] auto convert_tile_id_to_index(const Registry& registry,
                                            const Map& map,
                                            TileID tile_id) -> Maybe<TileIndex>;

/// Returns the attached tileset entity associated with a global tile ID.
[[nodiscard]] auto find_tileset_with_tile(const Registry& registry,
                                          const Map& map,
                                          TileID tile_id) -> Entity;

}  // namespace tactile::sys
