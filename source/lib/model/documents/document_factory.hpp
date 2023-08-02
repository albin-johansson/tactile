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

#include "common/type/path.hpp"
#include "core/ecs/entity.hpp"
#include "core/ecs/registry.hpp"
#include "core/math/vector.hpp"
#include "core/tiles/tile_extent.hpp"

namespace tactile::sys {

/**
 * Creates a map document.
 *
 * \note The created document is neither opened nor selected by this function.
 *
 * \param model     the associated registry.
 * \param extent    the size of the map.
 * \param tile_size the logical size of tiles in the map.
 *
 * \return a map document entity.
 */
auto create_map_document(Registry& registry,
                         const TileExtent& extent,
                         const Int2& tile_size) -> Entity;

/**
 * Creates a tileset document.
 *
 * \note The created document is neither opened nor selected by this function.
 *
 * \param model      the associated registry.
 * \param tile_size  the size of the tiles in the tileset image.
 * \param image_path the path to the source image.
 *
 * \return a tileset document entity.
 */
auto create_tileset_document(Registry& registry,
                             const Int2& tile_size,
                             const Path& image_path) -> Entity;

}  // namespace tactile::sys
