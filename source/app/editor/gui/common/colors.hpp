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

#include <array>  // array

#include <centurion/color.hpp>
#include <imgui.h>

#include "tactile.hpp"

namespace tactile {

/**
 * \brief Converts a color into an array of normalized color components.
 *
 * \details The array values are stored in the order red/green/blue/alpha.
 *
 * \ingroup utils
 *
 * \param color the color that will be converted.
 *
 * \return an array of normalized color components.
 */
[[nodiscard]] constexpr auto color_to_array(const cen::color& color) noexcept
    -> std::array<float, 4>
{
  const auto r = color.norm_red();
  const auto g = color.norm_green();
  const auto b = color.norm_blue();
  const auto a = color.norm_alpha();
  return {r, g, b, a};
}

[[nodiscard]] constexpr auto color_to_u32(const cen::color& color) -> uint32
{
  return IM_COL32(color.red(), color.green(), color.blue(), color.alpha());
}

[[nodiscard]] inline auto darker(const cen::color& color) -> cen::color
{
  constexpr float factor = 0.8f;
  return cen::color::from_norm(factor * color.norm_red(),
                               factor * color.norm_green(),
                               factor * color.norm_blue(),
                               1.0f);
}

}  // namespace tactile
