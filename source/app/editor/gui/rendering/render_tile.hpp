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

#include <entt/entt.hpp>

#include "tactile.hpp"

struct ImVec2;

namespace tactile {

class GraphicsCtx;

/**
 * \brief Renders a single tile.
 *
 * \ingroup rendering
 *
 * \param graphics the graphics context that will be used.
 * \param registry the active registry.
 * \param tile the tile that will be rendered.
 * \param row the row coordinate of the tile.
 * \param column the column coordinate of the tile.
 */
void RenderTile(GraphicsCtx& graphics,
                const entt::registry& registry,
                tile_id tile,
                int32 row,
                int32 column);

}  // namespace tactile
