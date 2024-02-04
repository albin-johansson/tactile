// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/buttons.hpp"

#include <imgui.h>

#include "tactile/core/ui/common/alignment.hpp"
#include "tactile/core/ui/common/tooltips.hpp"
#include "tactile/foundation/debug/assert.hpp"

namespace tactile::core {

auto show_button(const char* label,
                 const char* tooltip,
                 const bool enabled,
                 const float width,
                 const float height) -> bool
{
  TACTILE_ASSERT(label != nullptr);

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
  TACTILE_ASSERT(label != nullptr);

  const auto button_width = calculate_item_width(label);
  center_next_item_horizontally(button_width);

  return show_button(label, tooltip);
}

auto show_icon_button(const char* icon, const char* tooltip, const bool enabled) -> bool
{
  TACTILE_ASSERT(icon != nullptr);

  const auto& style = ImGui::GetStyle();
  const auto font_size = ImGui::GetFontSize();

  const auto width = font_size + (style.FramePadding.x * 2.0f);
  const auto height = font_size + (style.FramePadding.y * 2.0f);

  return show_button(icon, tooltip, enabled, width, height);
}

auto show_icon_button(const IconID icon_id, const char* tooltip, const bool enabled)
    -> bool
{
  return show_icon_button(get_icon_string(icon_id), tooltip, enabled);
}

}  // namespace tactile::core
