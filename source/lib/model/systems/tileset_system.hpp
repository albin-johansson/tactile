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
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "model/model.hpp"

namespace tactile::sys {

[[nodiscard]] auto is_tile_entity(const Model& model, Entity entity) -> bool;
[[nodiscard]] auto is_tileset_entity(const Model& model, Entity entity) -> bool;
[[nodiscard]] auto is_attached_tileset_entity(const Model& model, Entity entity) -> bool;

[[nodiscard]] auto create_tileset(Model& model,
                                  const Int2& tile_size,
                                  const Path& image_path) -> Entity;

[[nodiscard]] auto create_attached_tileset(Model& model,
                                           Entity tileset_entity,
                                           TileID first_tile) -> Entity;

[[nodiscard]] auto get_tile_appearance(const Model& model,
                                       Entity tileset_entity,
                                       TileIndex tile_index) -> TileIndex;

void make_tile_animated(Model& model, Entity tile_entity);

[[nodiscard]] auto is_valid_tile_identifier(const Model& model,
                                            Entity map_entity,
                                            TileID tile_id) -> bool;

}  // namespace tactile::sys
