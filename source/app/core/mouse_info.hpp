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

#include <centurion/mouse.hpp>

#include "tile_pos.hpp"

namespace tactile {

/**
 * \brief Provides general information about the mouse, used in response to mouse events.
 *
 * \todo Button might not be set.
 *
 * \ingroup core
 */
struct MouseInfo final
{
  float x{};                     ///< The mouse x-coordinate.
  float y{};                     ///< The mouse y-coordinate.
  TilePos position_in_viewport;  ///< The hovered tile position.
  cen::mouse_button button{};    ///< The activated mouse button.
  bool is_within_contents{};     ///< Is the mouse within the tile contents?
};

}  // namespace tactile
