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

/**
 * \brief A context component that is used to keep track of rectangle tool strokes.
 *
 * \ingroup components
 */
struct CurrentRectangleStroke final
{
  Vector2f start{};
  Vector2f current{};
};

/**
 * \brief A context component that is used to keep track of ellipse tool strokes.
 *
 * \ingroup components
 */
struct CurrentEllipseStroke final
{
  Vector2f start{};
  Vector2f current{};
};

}  // namespace tactile::comp
