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

#include <algorithm>  // find, min, max
#include <array>      // array
#include <cmath>      // pow

#include "editor/ui/conversions.hpp"
#include "editor/ui/style/themes.hpp"
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

inline std::array<bool, ImGuiCol_COUNT> current_colors;

}  // namespace

void update_dynamic_color_cache()
{
  const auto& style = ImGui::GetStyle();

  usize index = 0;
  for (const auto& color: style.Colors) {
    current_colors.at(index) = is_dark(to_color(color));
    ++index;
  }
}

auto make_brighter(const ImVec4& color, const float exp) -> ImVec4
{
  const auto factor = std::pow(1.20f, exp);
  return {std::min(1.0f, color.x * factor),
          std::min(1.0f, color.y * factor),
          std::min(1.0f, color.z * factor),
          1.0f};
}

auto make_darker(const ImVec4& color, const float exp) -> ImVec4
{
  const auto factor = std::pow(0.80f, exp);
  return {std::max(0.0f, color.x * factor),
          std::max(0.0f, color.y * factor),
          std::max(0.0f, color.z * factor),
          1.0f};
}

auto luminance(const cen::color& color) -> float
{
  const auto r_lin = to_linear(color.norm_red());
  const auto g_lin = to_linear(color.norm_green());
  const auto b_lin = to_linear(color.norm_blue());
  return (0.2126f * r_lin) + (0.7152f * g_lin) + (0.0722f * b_lin);
}

auto is_dark(const ImGuiCol color) -> bool
{
  return current_colors.at(static_cast<usize>(color));
}

auto is_dark(const cen::color& color) -> bool
{
  return luminance(color) < 0.3f;
}

}  // namespace tactile::ui
