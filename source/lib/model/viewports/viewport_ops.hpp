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

#include "common/type/math.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile::sys {

void offset_viewport(Viewport& viewport, Float2 delta);

void reset_viewport_zoom(Viewport& viewport);

void increase_viewport_zoom(Viewport& viewport, Float2 anchor_pos);

void decrease_viewport_zoom(Viewport& viewport, Float2 anchor_pos);

[[nodiscard]] auto get_scaling_ratio(const Viewport& viewport,
                                     const Float2& logical_tile_size) -> Float2;

[[nodiscard]] auto can_zoom_out_in(const Viewport& viewport) -> bool;

}  // namespace tactile::sys
