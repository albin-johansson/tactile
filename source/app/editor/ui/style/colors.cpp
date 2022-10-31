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

#include "colors.hpp"

#include <cmath>  // pow

#include "meta/build.hpp"

namespace tactile::ui {
namespace {

// https://en.wikipedia.org/wiki/SRGB#From_sRGB_to_CIE_XYZ
[[nodiscard]] auto to_linear(const float channel) noexcept(TACTILE_COMPILER_MSVC) -> float
{
  if (channel <= 0.04045f) {
    return channel / 12.92f;
  }
  else {
    return std::pow(((channel + 0.055f) / 1.055f), 2.4f);
  }
}

}  // namespace

auto luminance(const cen::color& color) -> float
{
  const auto r_lin = to_linear(color.norm_red());
  const auto g_lin = to_linear(color.norm_green());
  const auto b_lin = to_linear(color.norm_blue());
  return (0.2126f * r_lin) + (0.7152f * g_lin) + (0.0722f * b_lin);
}

auto is_dark(const cen::color& color) -> bool
{
  return luminance(color) < 0.40f;
}

}  // namespace tactile::ui
