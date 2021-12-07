#include "alignment.hpp"

#include <imgui_internal.h>

namespace Tactile {

void PrepareVerticalAlignmentCenter(const float count)
{
  const auto fontSize = ImGui::GetFontSize();

  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2{2.0f, 2.0f};

  const auto padding = ImGui::GetStyle().FramePadding;
  const auto height = (count * (fontSize + (2.0f * padding.y)));

  const auto cursorPos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursorPos + ImVec2{0, halfRegion.y - (height / 2.0f)});
}

void AlignNextItemCenteredHorizontally(const float width)
{
  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2{2.0f, 2.0f};
  ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2{halfRegion.x - (width / 2.0f), 0});
}

void CenterNextWindowOnAppearance()
{
  const auto* viewport = ImGui::GetMainViewport();
  const auto pos = viewport->WorkPos;
  const auto size = viewport->WorkSize;

  const ImVec2 next_pos = pos + (size / ImVec2{2, 2});
  const ImVec2 next_pivot{0.5f, 0.5f};

  ImGui::SetNextWindowPos(next_pos, ImGuiCond_Appearing, next_pivot);
}

}  // namespace Tactile
