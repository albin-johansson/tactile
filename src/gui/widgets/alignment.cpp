#include "alignment.hpp"

#include <imgui.h>
#include <imgui_internal.h>

namespace Tactile {

void PrepareVerticalAlignmentCenter(const float count)
{
  const auto fontSize = ImGui::GetFontSize();

  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2{2.0f, 2.0f};

  const auto padding = ImGui::GetStyle().FramePadding;
  const auto spacing = ImGui::GetStyle().ItemSpacing;

  const auto height = (count * (fontSize + (2.0f * padding.y)));

  const auto cursorPos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursorPos + ImVec2{0, halfRegion.y - (height / 2.0f)});
}

void AlignNextItemCenteredHorizontally(const float width)
{
  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2{2.0f, 2.0f};
  ImGui::SetCursorPos(ImGui::GetCursorPos() +
                      ImVec2{halfRegion.x - (width / 2.0f), 0});
}

}  // namespace Tactile
