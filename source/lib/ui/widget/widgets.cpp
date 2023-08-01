/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "widgets.hpp"

#include <imgui.h>

#include "common/type/chrono.hpp"
#include "core/containers/hash_map.hpp"
#include "core/debug/assert.hpp"
#include "core/functional/maybe.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {

auto push_button(const char* text,
                 const char* tooltip,
                 const bool enabled,
                 const float width,
                 const float height) -> bool
{
  TACTILE_ASSERT(text);

  const Disable disable {!enabled};

  const auto result = ImGui::Button(text, {width, height});

  if (tooltip) {
    push_lazy_tooltip(text, tooltip);
  }

  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
  }

  return result;
}

auto push_icon_button(const char* icon, const char* tooltip, const bool enabled) -> bool
{
  const auto& style = ImGui::GetStyle();
  const float height = ImGui::GetFontSize() + style.ItemSpacing.y * 2.0f;
  const float width = height;
  return push_button(icon, tooltip, enabled, width, height);
}

auto push_centered_button(const char* text, const char* tooltip) -> bool
{
  TACTILE_ASSERT(text);

  const auto width = standard_component_width(text);
  center_next_item_horizontally(width);

  return push_button(text, tooltip);
}

auto push_checkbox(const char* label, bool* value, const char* tooltip) -> bool
{
  TACTILE_ASSERT(label);
  TACTILE_ASSERT(value);
  const auto changed = ImGui::Checkbox(label, value);

  if (tooltip) {
    push_lazy_tooltip(label, tooltip);
  }

  return changed;
}

void push_lazy_tooltip(const char* id, const char* tooltip)
{
  TACTILE_ASSERT(id);
  TACTILE_ASSERT(tooltip);

  static HashMap<ImGuiID, Maybe<TimePoint>> state;

  const auto hashed_id = ImGui::GetID(id);
  auto& last_hover = state[hashed_id];

  if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
    if (!last_hover.has_value()) {
      last_hover = Clock::now();
    }

    using namespace std::chrono_literals;
    if (Clock::now() - last_hover.value() > 1s) {
      ImGui::SetTooltip("%s", tooltip);
    }
  }
  else {
    last_hover.reset();
  }
}

void push_centered_label(const char* text)
{
  TACTILE_ASSERT(text);
  const auto text_size = ImGui::CalcTextSize(text);

  const auto region = ImGui::GetContentRegionAvail();
  const auto half_region = region * ImVec2 {0.5f, 0.5f};

  const auto cursor_pos = ImGui::GetCursorPos();
  ImGui::SetCursorPos(cursor_pos + ImVec2 {half_region.x - (text_size.x * 0.5f), 0});
  ImGui::TextUnformatted(text);
}

}  // namespace tactile::ui
