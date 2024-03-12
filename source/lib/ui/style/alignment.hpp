// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>  // max

#include <imgui.h>

namespace tactile::ui {

/// Makes the next window centered when it is first shown.
void center_next_window_on_appearance();

/// Centers the next item horizontally, given its width.
///
/// \param width the width of the component.
void center_next_item_horizontally(float width);

/// Ensures that a specific amount of components will be centered vertically.
///
/// \param count the amount of items to be centered.
void prepare_vertical_alignment_center(float count = 1);

/// Prepares the widget cursor for a right-aligned component.
void right_align_next_item();

/// Prepares the widget cursor for a right-aligned component.
///
/// \param width the width of the component.
/// \return the new cursor x-coordinate.
auto right_align_next_item(float width) -> float;

/// Prepares the cursor for a right-aligned standard component with a label.
///
/// Use the `standard_component_width()` to query expected component widths
/// based on the label of the component.
///
/// \param text the component label text.
/// \return the new cursor x-coordinate.
auto right_align_next_item(const char* text) -> float;

/// Returns the width of a "standard" component with a label, e.g. a button.
///
/// \param text the component label text.
/// \return the component width.
[[nodiscard]] auto standard_component_width(const char* text) -> float;

/// Determines the minimum offset to align a collection of strings.
///
/// \param strings a pack of strings.
/// \return the minimum offset.
[[nodiscard]] auto minimum_offset_to_align(auto&&... strings) -> float
{
  const auto spacing = ImGui::GetStyle().ItemSpacing.x * 2.0f;
  return spacing + std::max({ImGui::CalcTextSize(strings).x...});
}

}  // namespace tactile::ui
