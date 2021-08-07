#include "tool_button.hpp"

#include <imgui.h>

#include "gui/widgets/common/button.hpp"

namespace Tactile {

auto ToolButton(const czstring label,
                const czstring tooltip,
                const bool selected,
                const bool enabled = true) -> bool
{
  if (selected)
  {
    const auto color = ImVec4{0, 0.7f, 0, 1.0f};
    ImGui::PushStyleColor(ImGuiCol_Button, color);
  }

  const auto pressed = Button(label, tooltip, enabled);

  if (selected)
  {
    ImGui::PopStyleColor();
  }

  return pressed;
}

}  // namespace Tactile
