#pragma once

#include <algorithm>  // algorithm

#include <imgui.h>

namespace Tactile {

// Ensures that the specified amount of items are centered in the window
void PrepareVerticalAlignmentCenter(float count = 1);

void AlignNextItemCenteredHorizontally(float width);

void CenterNextWindowOnAppearance();

template <typename... Strings>
[[nodiscard]] auto GetMinimumOffsetToAlign(Strings&&... strings) -> float
{
  const auto spacing = ImGui::GetStyle().ItemSpacing.x * 2.0f;
  return spacing + std::max({ImGui::CalcTextSize(strings).x...});
}

}  // namespace Tactile
