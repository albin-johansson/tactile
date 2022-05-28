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

#include <algorithm>  // max

#include <imgui.h>

namespace tactile::ui {

/// \addtogroup gui
/// \{

/**
 * \brief Makes the next window centered when it is first shown.
 */
void center_next_window_on_appearance();

/**
 * \brief Centers the next item horizontally, given its width.
 *
 * \param width the width of the component.
 */
void center_next_item_horizontally(float width);

/**
 * \brief Ensures that a certain amount of components will be centered vertically.
 *
 * \param count the amount of items to be centered.
 */
void prepare_vertical_alignment_center(float count = 1);

/**
 * \brief Prepares the cursor for a right-aligned component.
 *
 * \param width the width of the component.
 *
 * \return the new cursor x-coordinate.
 */
auto right_align_next_item(float width) -> float;

/**
 * \brief Prepares the cursor for a right-aligned standard component with a label.
 *
 * \param text the component label text.
 *
 * \return the new cursor x-coordinate.
 *
 * \see standard_component_width()
 */
auto right_align_next_item(const char* text) -> float;

/**
 * \brief Returns the width of a "standard" component with a label, e.g. a button.
 *
 * \param text the component label text.
 *
 * \return the component width.
 */
[[nodiscard]] auto standard_component_width(const char* text) -> float;

/**
 * \brief Determines the minimum offset to align a collection of strings.
 *
 * \param strings a pack of strings.
 *
 * \return the minimum offset.
 */
[[nodiscard]] auto minimum_offset_to_align(auto&&... strings) -> float
{
  const auto spacing = ImGui::GetStyle().ItemSpacing.x * 2.0f;
  return spacing + (std::max)({ImGui::CalcTextSize(strings).x...});
}

/// \} End of group gui

}  // namespace tactile::ui
