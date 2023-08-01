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
#include "core/prelude.hpp"
#include "core/tiles/tile_extent.hpp"
#include "model/registry.hpp"

namespace tactile::sys {

/**
 * Creates a new tile layer.
 *
 * \details The created entity will feature the following components: `Layer`,
 *          `TileLayer`, and `Context`.
 *
 * \param model  the associated registry.
 * \param id     the identifier associated with the layer.
 * \param extent the initial size of the layer.
 *
 * \return a tile layer entity.
 */
[[nodiscard]] auto create_tile_layer(Registry& registry, int32 id, TileExtent extent)
    -> Entity;

/**
 * Creates a new object layer.
 *
 * \details The created entity will feature the following components: `Layer`,
 *          `ObjectLayer`, and `Context`.
 *
 * \param model  the associated registry.
 * \param id     the identifier associated with the layer.
 *
 * \return an object layer entity.
 */
[[nodiscard]] auto create_object_layer(Registry& registry, int32 id) -> Entity;

/**
 * Creates a new group layer.
 *
 * \details The created entity will feature the following components: `Layer`,
 *          `GroupLayer`, and `Context`.
 *
 * \param model  the associated registry.
 * \param id     the identifier associated with the layer.
 *
 * \return a group layer entity.
 */
[[nodiscard]] auto create_group_layer(Registry& registry, int32 id) -> Entity;

}  // namespace tactile::sys
