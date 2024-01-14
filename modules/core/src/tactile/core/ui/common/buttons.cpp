// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/buttons.hpp"

#include <imgui.h>

#include "tactile/core/ui/common/alignment.hpp"
#include "tactile/core/ui/common/tooltips.hpp"
#include "tactile/foundation/debug/assert.hpp"

namespace tactile::ui {

auto show_button(const char* label,
                 const char* tooltip,
                 const bool enabled,
                 const float width,
                 const float height) -> bool
{
  TACTILE_ASSERT(label);

  ImGui::BeginDisabled(!enabled);

  const auto result = ImGui::Button(label, ImVec2 {width, height});

  if (tooltip) {
    lazy_tooltip(label, tooltip);
  }

  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  }

  ImGui::EndDisabled();

  return result;
}

auto show_centered_button(const char* label, const char* tooltip) -> bool
{
  TACTILE_ASSERT(label);

  const auto button_width = calculate_item_width(label);
  center_next_item_horizontally(button_width);

  return show_button(label, tooltip);
}

}  // namespace tactile::ui
