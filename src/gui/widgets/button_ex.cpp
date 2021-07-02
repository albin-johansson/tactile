#include "button_ex.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "gui/cursors.hpp"

namespace Tactile {

auto ButtonEx(const czstring text, const czstring tooltip, const bool enabled)
    -> bool
{
  ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !enabled);

  const auto result = ImGui::Button(text);

  if (tooltip)
  {
    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
    {
      ImGui::SetTooltip("%s", tooltip);
      EnableCursor(cen::system_cursor::hand);
    }
  }

  ImGui::PopItemFlag();

  return result;
}

}  // namespace Tactile
