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

#include <centurion/color.hpp>

#include "core/type/array.hpp"
#include "core/vocabulary.hpp"

namespace tactile::ui {

/// Converts a color into an array of normalized color components.
/// The array values are stored in the order red/green/blue/alpha.
[[nodiscard]] constexpr auto color_to_array(const cen::color& color) noexcept
    -> Array<float, 4>
{
  const auto r = color.norm_red();
  const auto g = color.norm_green();
  const auto b = color.norm_blue();
  const auto a = color.norm_alpha();
  return {r, g, b, a};
}

/// Computes the relative luminance of a color.
///
/// \details
/// This function is useful when determining appropriate text color for a background
/// of a certain color.
///
/// \see
/// https://en.wikipedia.org/wiki/Relative_luminance
///
/// \param color the color to compute the luminance for.
/// \return the color luminance, in the interval [0, 1].
[[nodiscard]] auto luminance(const cen::color& color) -> float;

/// Provides a heuristic indication for whether a color is dark or bright.
[[nodiscard]] auto is_dark(const cen::color& color) -> bool;

}  // namespace tactile::ui
