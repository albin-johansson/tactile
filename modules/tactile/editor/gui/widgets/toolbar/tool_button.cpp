#include "tool_button.hpp"

#include <imgui.h>

#include "tactile/editor/gui/widgets/common/button.hpp"

namespace Tactile {

auto ToolButton(const CStr label,
                const CStr tooltip,
                const bool selected,
                const bool enabled) -> bool
{
  if (selected) {
    const auto& style = ImGui::GetStyle();
    ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_TabActive]);
  }

  const auto pressed = Button(label, tooltip, enabled);

  if (selected) {
    ImGui::PopStyleColor();
  }

  return pressed;
}

}  // namespace Tactile
