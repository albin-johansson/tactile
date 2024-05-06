// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/buttons.hpp"

#include <imgui.h>

#include "tactile/core/ui/common/widgets.hpp"

namespace tactile::ui {

auto push_button(const char* label,
                 const char* tooltip,
                 const bool enabled,
                 const float width,
                 const float height) -> bool
{
  const DisabledScope disable {!enabled};

  const auto was_pressed = ImGui::Button(label, ImVec2 {width, height});

  if (ImGui::IsItemHovered()) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    if (tooltip != nullptr) {
      ImGui::SetTooltip("%s", tooltip);
    }
  }

  return was_pressed;
}

auto push_icon_button(const Icon icon,
                      const char* tooltip,
                      const bool enabled) -> bool
{
  const auto* icon_string = to_string(icon);

  const auto& style = ImGui::GetStyle();
  const auto side_length = ImGui::GetFontSize() + style.ItemSpacing.y * 2.0f;

  return push_button(icon_string, tooltip, enabled, side_length, side_length);
}

}  // namespace tactile::ui
