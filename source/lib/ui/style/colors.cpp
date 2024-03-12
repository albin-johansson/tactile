// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "colors.hpp"

#include <algorithm>  // find, min, max
#include <cmath>      // pow

#include "tactile/base/container/array.hpp"
#include "tactile/base/prelude.hpp"
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
