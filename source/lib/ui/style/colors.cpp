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
#include <cmath>      // pow

#include "common/predef.hpp"
#include "common/type/array.hpp"
#include "themes.hpp"
#include "ui/conversions.hpp"

namespace tactile::ui {
namespace {

inline Array<bool, ImGuiCol_COUNT> gCurrentColors;

}  // namespace

void update_dynamic_color_cache()
{
  const auto& style = ImGui::GetStyle();

  usize index = 0;
  for (const auto& color: style.Colors) {
    gCurrentColors.at(index) = to_color(color).is_dark();
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

}  // namespace tactile::ui
