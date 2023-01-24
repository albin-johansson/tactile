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

#include "common/macros.hpp"
#include "common/numeric.hpp"

struct ImVec2;

TACTILE_FWD_DECLARE_CLASS_NS(tactile, Map)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilePos)

namespace tactile::ui {

TACTILE_FWD_DECLARE_CLASS(Graphics)

void render_tile(Graphics& graphics,
                 const Map& map,
                 TileID tile_id,
                 const TilePos& pos,
                 float opacity);

}  // namespace tactile::ui
