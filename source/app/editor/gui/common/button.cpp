#include "button.hpp"

#include <imgui.h>

#include "editor/gui/scoped.hpp"
#include "misc/assert.hpp"

namespace tactile {

auto Button(const c_str text,
            const c_str tooltip,
            const bool enabled,
            const float width,
            const float height) -> bool
{
  TACTILE_ASSERT(text);

  const scoped::Disable disable{!enabled};

  const auto result = ImGui::Button(text, {width, height});

  if (tooltip) {
    if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
      ImGui::SetTooltip("%s", tooltip);
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
  }

  return result;
}

}  // namespace tactile
