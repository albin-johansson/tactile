#pragma once

#include <algorithm>  // algorithm

#include <imgui.h>

#include "tactile.hpp"

namespace tactile {

void CenterNextWindowOnAppearance();

// Ensures that the specified amount of items are centered in the window
void PrepareVerticalAlignmentCenter(float count = 1);

void AlignNextItemCenteredHorizontally(float width);

// Adjusts the cursor for a right-aligned component, returning the new cursor x-coordinate
auto AlignNextItemToTheRight(float width) -> float;

auto AlignNextItemToTheRight(c_str text) -> float;

[[nodiscard]] auto GetStandardComponentWidth(const char* text) -> float;

template <typename... Strings>
[[nodiscard]] auto GetMinimumOffsetToAlign(Strings&&... strings) -> float
{
  const auto spacing = ImGui::GetStyle().ItemSpacing.x * 2.0f;
  return spacing + ((std::max))({ImGui::CalcTextSize(strings).x...});
}

}  // namespace tactile
