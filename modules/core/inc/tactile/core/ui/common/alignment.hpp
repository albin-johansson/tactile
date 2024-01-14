// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <algorithm>         // max
#include <concepts>          // convertible_to
#include <initializer_list>  // initializer_list

#include <imgui.h>

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::ui {

/**
 * Returns the width of a "standard" item with a label, e.g., a button.
 *
 * \param text The item label.
 *
 * \return
 *    The computed width.
 */
TACTILE_CORE_API auto calculate_item_width(const char* text) -> float;

/**
 * Prepare the widget cursor for a right-aligned item.
 */
TACTILE_CORE_API void right_align_next_item();

/**
 * Prepares the widget cursor for a right-aligned item.
 *
 * \param item_width The item width.
 *
 * \return
 *    The new cursor x-coordinate.
 */
TACTILE_CORE_API auto right_align_next_item(float item_width) -> float;

/**
 * Prepares the cursor for a right-aligned standard sized item with a label.
 *
 * \note
 *    Use the `calculate_item_width` function to determine expected items widths.
 *
 * \param text The item label.
 *
 * \return
 *    The new cursor x-coordinate.
 */
TACTILE_CORE_API auto right_align_next_item(const char* text) -> float;

/**
 * Centers the next item horizontally, given its width.
 *
 * \param item_width The item width.
 */
TACTILE_CORE_API void center_next_item_horizontally(float item_width);

/**
 * Makes the next window appear centered when it is first shown.
 */
TACTILE_CORE_API void center_next_window_on_appearance();

/**
 * Ensures that the specified number of components will be vertically centered.
 *
 * \param item_count The number of items.
 */
TACTILE_CORE_API void prepare_for_vertically_center_aligned_items(float item_count);

/**
 * Computes the minimum offset necessary to vertically align widgets to the right of their labels.
 *
 * \param labels The labels to measure.
 *
 * \return
 *    The computed offset.
 */
template <std::convertible_to<const char*>... Strings>
[[nodiscard]] auto minimum_offset_to_align(Strings... labels) -> float
{
  const auto spacing = ImGui::GetStyle().ItemSpacing.x * 2.0f;
  return spacing +
         std::max(std::initializer_list<float> {ImGui::CalcTextSize(labels).x...});
}

}  // namespace tactile::ui
