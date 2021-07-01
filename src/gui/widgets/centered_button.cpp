#include "centered_button.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace Tactile {

auto CenteredButton(const czstring text) -> bool
{
  const auto textSize = ImGui::CalcTextSize(text);

  const auto region = ImGui::GetContentRegionAvail();
  const auto halfRegion = region / ImVec2{2.0f, 2.0f};
  const auto padding = ImGui::GetStyle().FramePadding;

  const auto width = textSize.x + (padding.x * 2.0f);

  const auto cursorPos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursorPos + ImVec2{halfRegion.x - (width / 2.0f), 0});

  return ImGui::Button(text);
}

}  // namespace Tactile
