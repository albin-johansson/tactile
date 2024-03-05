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

#pragma once

#include <imgui.h>

#include "core/color.hpp"
#include "tactile/base/container/array.hpp"

namespace tactile::ui {

/// Updates the cache of color data associated with the current theme.
/// This is used to limit relatively expensive color luminance computations.
void update_dynamic_color_cache();

/// Copies a given color, using a different alpha component.
[[nodiscard]] constexpr auto with_alpha(const ImVec4& color, const float alpha) noexcept
    -> ImVec4
{
  return {color.x, color.y, color.z, alpha};
}

/// Makes a given color brighter increasing all component values.
[[nodiscard]] auto make_brighter(const ImVec4& color, float exp = 1.0) -> ImVec4;

/// Makes a given color darker by decreasing all component values.
[[nodiscard]] auto make_darker(const ImVec4& color, float exp = 1.0) -> ImVec4;

/// Indicates whether a style color is dark.
/// Note, this function references an internal color cache, which must be updated
/// with the update_dynamic_color_cache function.
[[nodiscard]] auto is_dark(ImGuiCol color) -> bool;

}  // namespace tactile::ui
