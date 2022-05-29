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

#include "core/common/math.hpp"

namespace tactile::comp {

/// Optional component used to limit viewport offsets.
struct ViewportLimits final
{
  Vector2f min_offset{};
  Vector2f max_offset{};
};

/// Component that provides information about a viewport.
struct Viewport final
{
  Vector2f offset{};     ///< The viewport offset.
  Vector2f tile_size{};  ///< The graphical tile size used in the viewport.
};

}  // namespace tactile::comp