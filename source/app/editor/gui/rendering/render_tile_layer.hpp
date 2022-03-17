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

namespace tactile {

class GraphicsCtx;

/**
 * \brief Renders a tile layer.
 *
 * \ingroup rendering
 *
 * \param graphics the graphics context that will be used.
 * \param registry the current registry.
 * \param layerEntity the entity that represents the tile layer.
 * \param parentOpacity the opacity of the parent layer.
 */
void render_tile_layer(GraphicsCtx& graphics,
                       const entt::registry& registry,
                       entt::entity layerEntity,
                       float parentOpacity);

}  // namespace tactile
