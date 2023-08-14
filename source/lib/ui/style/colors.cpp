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

#include "colors.hpp"

#include <algorithm>  // min, max
#include <cmath>      // pow

#include "tactile/core/containers/array.hpp"
#include "core/prelude.hpp"
#include "themes.hpp"
#include "ui/conversions.hpp"

namespace tactile {
namespace {

inline Array<bool, ImGuiCol_COUNT> gCurrentColors;

}  // namespace

void update_dynamic_color_cache()
{
  const auto& style = ImGui::GetStyle();

  usize index = 0;
  for (const auto& color: style.Colors) {
    gCurrentColors.at(index) = is_dark_color(normalize(to_color(color)));
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

auto is_dark(const ImGuiCol color) -> bool
{
  return gCurrentColors.at(static_cast<usize>(color));
}

}  // namespace tactile
