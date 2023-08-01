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

#include "common/region.hpp"
#include "core/functional/maybe.hpp"

struct ImVec2;

namespace tactile::ui {

/**
 * Updates a "rubber band" (rectangular selection) in the current canvas.
 *
 * \param scroll_offset the current viewport scroll offset.
 * \param tile_size     the logical tile size.
 *
 * \return the current selection; nothing there is none.
 */
[[nodiscard]] auto ui_rubber_band(const ImVec2& scroll_offset, const ImVec2& tile_size)
    -> Maybe<Region>;

}  // namespace tactile::ui
