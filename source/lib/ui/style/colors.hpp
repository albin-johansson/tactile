// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <imgui.h>

#include "tactile/base/container/array.hpp"
#include "tactile/core/meta/color.hpp"

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
