#include "button.hpp"

#include <imgui.h>

#include "editor/gui/alignment.hpp"
#include "editor/gui/scoped.hpp"
#include "misc/assert.hpp"

namespace tactile {

auto button(const char* text,
            const char* tooltip,
            const bool enabled,
            const float width,
            const float height) -> bool
{
  TACTILE_ASSERT(text);

  const scoped::disable disable{!enabled};

  const auto result = ImGui::Button(text, {width, height});

  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    if (tooltip) {
      ImGui::SetTooltip("%s", tooltip);
    }

    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  }

  return result;
}

auto centered_button(const char* text, const char* tooltip) -> bool
{
  TACTILE_ASSERT(text);

  const auto width = standard_component_width(text);
  center_next_item_horizontally(width);

  return button(text, tooltip);
}

}  // namespace tactile
