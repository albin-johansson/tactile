#include "button.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <cassert>  // assert

namespace Tactile {

auto Button(const NotNull<CStr> text, const CStr tooltip, const bool enabled) -> bool
{
  assert(text);

  if (!enabled)
  {
    ImGui::BeginDisabled();
  }

  ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !enabled);

  const auto result = ImGui::Button(text);

  if (tooltip)
  {
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip("%s", tooltip);
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
  }

  ImGui::PopItemFlag();

  if (!enabled)
  {
    ImGui::EndDisabled();
  }

  return result;
}

}  // namespace Tactile
