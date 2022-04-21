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

#include <vector>  // vector

#include <entt/fwd.hpp>

#include "core/fwd.hpp"
#include "tactile.hpp"

namespace tactile {

/**
 * \brief Floods the specified tile layer with a tile at the specified location.
 *
 * \param registry the document registry.
 * \param entity the tile layer entity.
 * \param origin the origin position of the flood fill.
 * \param replacement the tile identifier used to replace the origin identifier.
 * \param[out] affected the positions of the tiles affected by the flood fill.
 */
void flood(entt::registry& registry,
           entt::entity entity,
           const TilePos& origin,
           TileID replacement,
           std::vector<TilePos>& affected);

}  // namespace tactile
