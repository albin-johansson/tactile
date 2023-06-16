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

#include "common/color.hpp"
#include "common/tile_pos.hpp"
#include "common/type/math.hpp"
#include "ui/render/canvas.hpp"

namespace tactile::ui {

void push_scissor(const CanvasInfo& canvas);
void pop_scissor();

void clear_canvas(const CanvasInfo& canvas, const Color& color);

void render_infinite_grid(const CanvasInfo& canvas, const Color& line_color);

void render_translated_grid(const CanvasInfo& canvas, const Color& line_color);

void render_outline(const CanvasInfo& canvas, const Color& outline_color);

/**
 * Renders a single tile from a tileset.
 *
 * \param canvas         the target canvas.
 * \param texture        a handle to the tileset texture.
 * \param uv_size        the size of each tile in the tileset image, in UV coordinates.
 * \param pos_in_tileset the position of the tile in the tileset.
 * \param rendered_pos   the position of the rendered tile.
 * \param opacity        the opacity of the tile, in the interval [0, 1].
 */
void render_tile_image(const CanvasInfo& canvas,
                       ImTextureID texture,
                       const Float2& uv_size,
                       const TilePos& pos_in_tileset,
                       const Float2& rendered_pos,
                       float opacity = 1.0f);

[[nodiscard]] auto translate_pos(const CanvasInfo& canvas, const ImVec2& position)
    -> ImVec2;

[[nodiscard]] auto translate_tile_pos(const CanvasInfo& canvas, const TilePos& tile_pos)
    -> Float2;

}  // namespace tactile::ui
