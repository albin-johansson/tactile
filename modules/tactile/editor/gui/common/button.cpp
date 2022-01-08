#include "button.hpp"

#include <imgui.h>

#include "assert.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

auto Button(const NotNull<CStr> text,
            const CStr tooltip,
            const bool enabled,
            const float width,
            const float height) -> bool
{
  TACTILE_ASSERT(text);

  const Scoped::Disable disable{!enabled};

  const auto result = ImGui::Button(text, {width, height});

  if (tooltip) {
    if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
      ImGui::SetTooltip("%s", tooltip);
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
  }

  return result;
}

}  // namespace Tactile
